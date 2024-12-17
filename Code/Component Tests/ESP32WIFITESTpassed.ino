#include <WiFi.h>

const char* ssid = "MYNETWORK";
const char* password = "MYPASSWORD";

void setup() {
  Serial.begin(115200);
  Serial.println("Connecting to Wi-Fi");
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWi-Fi connected!");
  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected! Reconnecting...");
    WiFi.begin(ssid, password);
    delay(5000);
  } else {
    Serial.println("Wi-Fi connected!");
  }
  delay(2000);
}
