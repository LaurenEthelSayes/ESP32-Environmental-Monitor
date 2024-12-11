#define SOUND_SENSOR_PIN 33  

void setup() {
  Serial.begin(115200); 
  pinMode(SOUND_SENSOR_PIN, INPUT); 
  Serial.println("testing sound sensor!");
}

void loop() {

  int soundLevel = analogRead(SOUND_SENSOR_PIN);

  Serial.print("Sound Level (Raw Value): ");
  Serial.println(soundLevel);

  delay(2000); 
}
