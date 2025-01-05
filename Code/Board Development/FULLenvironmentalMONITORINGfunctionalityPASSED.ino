#include <Wire.h>
#include <U8g2lib.h>
#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MQ135_AOUT 34
#define GREEN_LED 17
#define RED_LED 16
#define BUZZER_PIN 18
#define RELAY_PIN 19

const char* ssid = "MYWIFISSID";
const char* password = "MYWIFIPASSWORD";
const char* mqtt_server = "192.168.0.37";
const int mqtt_port = 1883;
const char* mqtt_topic = "iot/test/data";

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

bool fanState = false;

void setup() {
    Serial.begin(115200);
    dht.begin();
    u8g2.begin();

    pinMode(GREEN_LED, OUTPUT);
    pinMode(RED_LED, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(RELAY_PIN, LOW);

    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());

    mqttClient.setServer(mqtt_server, mqtt_port);

    Serial.println(" System Initialised: DHT22, MQ135, OLED, LEDs, Buzzer, and Relay Fan WITH MQTT!");
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.drawStr(0, 10, " Starting System");
    u8g2.sendBuffer();
}

void loop() {
    if (!mqttClient.connected()) {
        if (mqttClient.connect("ESP32Client")) {
            Serial.println("Connected to MQTT!");
        } else {
            Serial.print("Failed");
            Serial.println(mqttClient.state());
            delay(5000);
            return;
        }
    }

    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int air_quality_analog = analogRead(MQ135_AOUT);
    float air_quality_ppm = calculatePPM(air_quality_analog);

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println(" Failed to read from DHT22 sensor!");
        u8g2.clearBuffer();
        u8g2.drawStr(0, 10, " DHT22 failed");
        u8g2.sendBuffer();
        delay(2000);
        return;
    }

    bool tempSafe = (temperature >= 22.0 && temperature <= 24.0);
    bool humiditySafe = (humidity >= 40.0 && humidity <= 60.0);
    bool airQualitySafe = (air_quality_ppm <= 250);

    String status;
    String fanMessage;

    if (tempSafe && humiditySafe && airQualitySafe) {
        status = "Good Conditions";
        fanMessage = " Fan: Cancelled";
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(RED_LED, LOW);
        digitalWrite(RELAY_PIN, LOW);
        noTone(BUZZER_PIN);
        fanState = false;
    } else {
        status = " Bad Conditions!";
        fanMessage = " Fan: Initiated";
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
        digitalWrite(RELAY_PIN, HIGH);
        playSiren();
        fanState = true;
    }

    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" °C");

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    Serial.print("Air Quality (PPM): ");
    Serial.println(air_quality_ppm);

    Serial.print(" Status: ");
    Serial.println(status);
    Serial.println(fanMessage);

    updateOLED(temperature, humidity, air_quality_ppm, status, fanMessage);

    static unsigned long lastTime = 0;
    if (millis() - lastTime > 5000) {
        lastTime = millis();
        String payload = "{";
        payload += "\"temperature\": " + String(temperature, 1) + ",";
        payload += "\"humidity\": " + String(humidity, 1) + ",";
        payload += "\"air_quality\": " + String(air_quality_ppm, 1);
        payload += "}";
        mqttClient.publish(mqtt_topic, payload.c_str());
        Serial.println("Published: " + payload);
    }

    mqttClient.loop();
}

void updateOLED(float temp, float humidity, float airQuality, String status, String fanMessage) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);

    u8g2.setCursor(0, 10);
    u8g2.print(" Temp: ");
    u8g2.print(temp);
    u8g2.println(" C");

    u8g2.setCursor(0, 20);
    u8g2.print(" Humidity: ");
    u8g2.print(humidity);
    u8g2.println(" %");

    u8g2.setCursor(0, 30);
    u8g2.print(" Air Qual: ");
    u8g2.print(airQuality);
    u8g2.println(" ppm");

    u8g2.setCursor(0, 45);
    u8g2.print(" Status: ");
    u8g2.println(status);

    u8g2.setCursor(0, 55);
    u8g2.print(fanMessage);

    u8g2.sendBuffer();
}

void playSiren() {
    tone(BUZZER_PIN, 1000);
    delay(500);
    tone(BUZZER_PIN, 1500);
    delay(500);
    tone(BUZZER_PIN, 1000);
    delay(500);
    noTone(BUZZER_PIN);
    delay(2000);
}

float calculatePPM(int rawValue) {
    float voltage = (rawValue / 4095.0) * 3.3;
    float ppm = (voltage - 0.1) * 1000;
    return ppm < 0 ? 0 : ppm;
}
