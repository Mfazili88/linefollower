#include <SoftwareSerial.h>

#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

SoftwareSerial bt(2, 3); // RX, TX HC-05

// ===== SNELHEDEN =====
int minSpeed  = 80;
int baseSpeed = 170;
int maxSpeed  = 200;

int straightThreshold = 200;   // onder deze fout = recht stuk
int straightBoost     = 30;    // extra snelheid op rechte lijn

int sensors[6] = {A0, A1, A2, A3, A4, A5};
bool robotAan = false;

// ===== PID =====
float Kp = 0.18;
float Ki = 0.0;
float Kd = 1.4;

int lastError = 0;
float integral = 0;

// ===== LIVE DEBUG =====
bool liveDebug = false;
unsigned long lastDebugTime = 0;
const int debugInterval = 100; // ms (10 Hz)

// ===== MOTOREN =====
void motorLeft(int speed) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) {
    analogWrite(IN1, speed);
    analogWrite(IN2, 0);
  } else {
    analogWrite(IN1, 0);
    analogWrite(IN2, -speed);
  }
}

void motorRight(int speed) {
  speed = constrain(speed, -255, 255);
  if (speed >= 0) {
    analogWrite(IN3, speed);
    analogWrite(IN4, 0);
  } else {
    analogWrite(IN3, 0);
    analogWrite(IN4, -speed);
  }
}

void stopMotor() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

// ===== STATUS PRINT =====
void printStatus() {
  bt.print("Kp="); bt.print(Kp);
  bt.print(" Ki="); bt.print(Ki);
  bt.print(" Kd="); bt.print(Kd);
  bt.print(" Min="); bt.print(minSpeed);
  bt.print(" Base="); bt.print(baseSpeed);
  bt.print(" Max="); bt.print(maxSpeed);
  bt.print(" Live="); bt.println(liveDebug);
}

void setup() {
  Serial.begin(9600);
  bt.begin(9600);

  bt.println("Robot klaar");
  bt.println("s/x | p/i/d | b/n/m | l1/l0 | ?");
}

void loop() {
  // ===== BLUETOOTH =====
  if (bt.available()) {
    String cmd = bt.readStringUntil('\n');
    cmd.trim();

    if (cmd == "s") {
      robotAan = true;
      integral = 0;
      lastError = 0;
      bt.println("START");
    }
    else if (cmd == "x") {
      robotAan = false;
      stopMotor();
      bt.println("STOP");
    }
    else if (cmd.startsWith("p")) Kp = cmd.substring(1).toFloat();
    else if (cmd.startsWith("i")) Ki = cmd.substring(1).toFloat();
    else if (cmd.startsWith("d")) Kd = cmd.substring(1).toFloat();
    else if (cmd.startsWith("b")) baseSpeed = constrain(cmd.substring(1).toInt(), 0, 200);
    else if (cmd.startsWith("n")) minSpeed  = constrain(cmd.substring(1).toInt(), 0, 200);
    else if (cmd.startsWith("m")) maxSpeed  = constrain(cmd.substring(1).toInt(), 0, 255);
    else if (cmd == "l1") {
      liveDebug = true;
      bt.println("Live debug AAN");
    }
    else if (cmd == "l0") {
      liveDebug = false;
      bt.println("Live debug UIT");
    }
    else if (cmd == "?") printStatus();
  }

  if (!robotAan) {
    stopMotor();
    return;
  }

  // ===== SENSOR LEZEN =====
  long position = 0, total = 0;

  for (int i = 0; i < 6; i++) {
    int value = analogRead(sensors[i]);
    value = constrain(value, 300, 900);
    value = map(value, 300, 900, 0, 1000);

    position += (long)value * (i * 1000);
    total += value;
  }

  int error;
  if (total == 0) {
    error = lastError;
    integral = 0;
  } else {
    error = (position / total) - 2500;
  }

  // ===== PID =====
  integral += error;
  integral = constrain(integral, -500, 500);

  int derivative = error - lastError;
  lastError = error;

  float correction = (Kp * error) +
                     (Ki * integral) +
                     (Kd * derivative);

  correction = constrain(correction, -255, 255);

  // ===== DYNAMISCHE SNELHEID =====
  // ===== DYNAMISCHE SNELHEID (VERBETERD) =====
int absError = abs(error);
int dynamicSpeed;

// === RECHT STUK ===
if (absError < straightThreshold) {
  dynamicSpeed = baseSpeed + straightBoost;
}

// === BOCHT ===
else {
  dynamicSpeed = map(absError,
                     straightThreshold, 2000,
                     baseSpeed, minSpeed);
}

dynamicSpeed = constrain(dynamicSpeed, minSpeed, maxSpeed);

motorLeft(dynamicSpeed - correction);
motorRight(dynamicSpeed + correction);

  // ===== LIVE DEBUG OUTPUT =====
  if (liveDebug && millis() - lastDebugTime > debugInterval) {
    lastDebugTime = millis();
    bt.print("E="); bt.print(error);
    bt.print(" C="); bt.print(correction);
    bt.print(" S="); bt.print(dynamicSpeed);
    bt.print(" I="); bt.println(integral);
  }
}