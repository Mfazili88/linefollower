
void setup() {
  // Seriële poort naar PC
  Serial.begin(9600);
  // Seriële poort naar HC-05
  Serial1.begin(9600);

  Serial.println("HC-05 test gestart");
  Serial.println("Open een Bluetooth-terminal en verbind met HC-05");
}

void loop() {
  // Data van Bluetooth → USB
  if (Serial1.available()) {
    char c = Serial1.read();
    Serial.write(c);
  }

  // Data van USB → Bluetooth
  if (Serial.available()) {
    char c = Serial.read();
    Serial1.write(c);
  }
}