#define RELAY_PIN 19 

void setup() {
  pinMode(RELAY_PIN, OUTPUT); 
  Serial.begin(115200);
  Serial.println("Starting relay test");
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH); 
  Serial.println("Relay ON");
  delay(1000);                 

  digitalWrite(RELAY_PIN, LOW);  
  Serial.println("Relay OFF");
  delay(1000);                   
}
