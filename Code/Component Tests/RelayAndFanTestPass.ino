#define RELAY_PIN 19

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  Serial.begin(115200);
  Serial.println("Relay Module Test Starting...");
}

void loop() {
  Serial.println("Relay ON - Fan ON");
  digitalWrite(RELAY_PIN, HIGH);
  delay(2000);

  Serial.println("Relay OFF - Fan OFF");
  digitalWrite(RELAY_PIN, LOW);
  delay(2000);
}
