#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define MQ135_PIN 34
DHT dht(DHTPIN, DHTTYPE);


void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT22 and MQ135 Sensor Test Starting...");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  int airQualityRaw = analogRead(MQ135_PIN);
  float airQualityPPM = map(airQualityRaw, 0, 4095, 0, 1000);

  Serial.print("Air Quality (PPM): ");
  Serial.print(airQualityPPM);
  Serial.println(" ppm");

  delay(2000);
}
