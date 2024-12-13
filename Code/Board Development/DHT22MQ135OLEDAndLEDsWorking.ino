#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ135_AOUT 34
#define GREEN_LED 17
#define RED_LED 16

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);
    dht.begin();
    u8g2.begin();
    Serial.println("DHT22, MQ135, LEDs, and OLED initialised");

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);

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
        u8g2.drawStr(0, 10, "DHT22 failed!");
        u8g2.sendBuffer();
        delay(2000);
        return;
    }

    int air_quality_analog = analogRead(MQ135_AOUT);
    float air_quality_ppm = calculatePPM(air_quality_analog);

    bool tempSafe = (temperature >= 22.0 && temperature <= 24.0);
    bool humiditySafe = (humidity >= 40.0 && humidity <= 60.0);
    bool airQualitySafe = (air_quality_ppm <= 259);

    String status;

    if (tempSafe && humiditySafe && airQualitySafe) {
        status = "Good Quality";
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
    } else {
        status = "Bad Quality";
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Air Quality (PPM): ");
    Serial.println(air_quality_ppm);

    Serial.print("Status: ");
    Serial.println(status);

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

    u8g2.setCursor(0, 50);
    u8g2.print("Status: ");
    u8g2.println(status);

    u8g2.sendBuffer();
    delay(2000);
}

float calculatePPM(int rawValue) {
    float voltage = (rawValue / 4095.0) * 3.3;
    float ppm = (voltage - 0.1) * 1000;
    return ppm < 0 ? 0 : ppm;
}
