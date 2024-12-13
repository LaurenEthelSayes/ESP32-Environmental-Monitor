#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("DHT22 Sensor test");
}

void loop() {
  // reading temperature AND humidity from DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // are readings ok?
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 sensor failure!!");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C  Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }

  delay(2000); // waiting two seconds delay
}
