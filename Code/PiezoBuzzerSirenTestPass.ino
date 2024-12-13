#define BUZZER_PIN 18

void setup() {
  pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {
  tone(BUZZER_PIN, 1000); 
  delay(500);             
  tone(BUZZER_PIN, 1500); 
  delay(500);           
  tone(BUZZER_PIN, 1000);     
  delay(500);      
  noTone(BUZZER_PIN);
  delay(2000);
}
