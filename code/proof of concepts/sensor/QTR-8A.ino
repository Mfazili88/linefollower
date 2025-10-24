const int numSensors = 6;
const int sensorPins[numSensors] = {A0, A1, A2, A3, A4, A5};
int sensorValues[numSensors];

const int ledPin = 2; // LED_ON van QTR-8A verbonden met D2 (optioneel)

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // zet IR-LEDs aan

  Serial.println("QTR-8A Test Start");
  delay(500);
}

void loop() {
  // lees alle sensoren
  for (int i = 0; i < numSensors; i++) {
    sensorValues[i] = analogRead(sensorPins[i]);
  }

  // print de waarden
  for (int i = 0; i < numSensors; i++) {
    Serial.print(sensorValues[i]);
    Serial.print("\t"); // tab voor nette kolommen
  }
  Serial.println();

  delay(100); // kleine vertraging voor leesbaarheid
}
