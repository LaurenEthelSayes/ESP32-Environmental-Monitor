#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ135_AOUT 34

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT22 and MQ135 sensor test ititalised");
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("failed tor ead from sensor");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  int air_quality_analog = analogRead(MQ135_AOUT);
  float air_quality_ppm = calculatePPM(air_quality_analog);

  Serial.print("Air Quality (PPM): ");
  Serial.println(air_quality_ppm);

  delay(2000);
}

float calculatePPM(int rawValue) {
  float voltage = (rawValue / 4095.0) * 3.3;
  float ppm = (voltage - 0.1) * 1000;
  return ppm < 0 ? 0 : ppm;
}
