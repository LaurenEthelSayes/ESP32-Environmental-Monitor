#define MQ135_PIN 34  

void setup() {
  Serial.begin(115200); 
  pinMode(MQ135_PIN, INPUT);
  Serial.println("Testing MQ135 sensor");
}

void loop() {
  int airQualityRaw = analogRead(MQ135_PIN); //analog due to pins

  
  Serial.print("Air Quality (Raw Value): ");
  Serial.println(airQualityRaw);

  delay(2000); // waiting two seconds
}
