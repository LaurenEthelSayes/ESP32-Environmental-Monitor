#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "MYWIFISSID";       
const char* password = "MYWIFIPASSWORD";    
// myMQTTBROKERinformation
const char* mqtt_server = "192.168.0.37"; 
const int mqtt_port = 1883;
const char* mqtt_topic = "iot/test/data";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(115200);

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
}

void loop() {
  
  if (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT");
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Connected to MQTT!");
    } else {
      Serial.print("Failed");
      Serial.println(mqttClient.state());
      delay(5000);
      return; 
    }
  }


  static unsigned long lastTime = 0;
  if (millis() - lastTime > 5000) {
    lastTime = millis();
    mqttClient.publish(mqtt_topic, "Test message from ESP32!");
    Serial.println("Published: Test message from ESP32!");
  }

  mqttClient.loop();
}
