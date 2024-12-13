#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ135_AOUT 34

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  u8g2.begin();
  Serial.println("DHT22, MQ135, and OLED initialised");

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.drawStr(0, 10, "Starting...");
  u8g2.sendBuffer();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("failed to read from sensor");
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "DHT22 failed");
    u8g2.sendBuffer();
    delay(2000);
    return;
  }

  int air_quality_analog = analogRead(MQ135_AOUT);
  float air_quality_ppm = calculatePPM(air_quality_analog);

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Air Quality (PPM): ");
  Serial.println(air_quality_ppm);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setCursor(0, 10);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.println(" C");

  u8g2.setCursor(0, 20);
  u8g2.print("Humidity: ");
  u8g2.print(humidity);
  u8g2.println(" %");

  u8g2.setCursor(0, 30);
  u8g2.print("Air Qual: ");
  u8g2.print(air_quality_ppm);
  u8g2.println(" ppm");

  if (air_quality_ppm > 250) {
    u8g2.setCursor(0, 50);
    u8g2.println("Status: Bad Quality");
  } else {
    u8g2.setCursor(0, 50);
    u8g2.println("Status: Good Quality");
  }

  u8g2.sendBuffer();
  delay(2000);
}

float calculatePPM(int rawValue) {
  float voltage = (rawValue / 4095.0) * 3.3;
  float ppm = (voltage - 0.1) * 1000;
  return ppm < 0 ? 0 : ppm;
}
