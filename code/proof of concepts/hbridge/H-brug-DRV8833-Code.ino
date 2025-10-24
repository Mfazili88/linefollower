/*
  DRV8833 Testprogramma
  Arduino Leonardo
  Motor A: IN1 = D7, IN2 = D6
  Motor B: IN3 = D5, IN4 = D4
*/

#define IN1 7
#define IN2 6
#define IN3 5
#define IN4 4

void setup() {
  // Stel pinnen in als outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
  Serial.println("DRV8833 test start");
}

void loop() {
  Serial.println("Beide motoren vooruit");
  vooruit();
  delay(2000);

  Serial.println("Stop");
  stopMotors();
  delay(1000);

  Serial.println("Beide motoren achteruit");
  achteruit();
  delay(2000);

  Serial.println("Stop");
  stopMotors();
  delay(1000);

  Serial.println("Links vooruit, rechts achteruit (draai links)");
  draaiLinks();
  delay(2000);

  Serial.println("Rechts vooruit, links achteruit (draai rechts)");
  draaiRechts();
  delay(2000);
}

// Functies
void vooruit() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void achteruit() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void draaiLinks() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void draaiRechts() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}