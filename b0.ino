
#include <Arduino.h>
int led = 2;
#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>
#include <HTTPUpdate.h>

const char* ssid = "Tenda_5DF7";
const char* password = "41182056";

const char* otaUrl = "https://raw.githubusercontent.com/Aomaytus/rmuti_iot/main/b0.ino.esp32.bin";

void setup() {
  Serial.begin(115200);
pinMode(led,OUTPUT);
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Load and update firmware
  updateFirmware();
  
}

void loop() {
//  digitalWrite(led,0);
//  delay(500);
//  digitalWrite(led,1);
//  delay(500);
  // Additional code or tasks to perform after the update is complete (if needed)
}

void updateFirmware() {
  HTTPClient http;

  http.begin(otaUrl);

  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    WiFiClient client;
    String url = http.getString();
    int contentLength = http.getSize();
    Serial.println(contentLength / 1000);
    http.end();

    t_httpUpdate_return ret = httpUpdate.update(client, url);

    switch (ret) {
      case HTTP_UPDATE_FAILED:
        Serial.println("HTTP update failed");
        break;
      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("No OTA updates available");
        break;
      case HTTP_UPDATE_OK:
        Serial.printf("OTA update successful, downloaded file size: %d bytes\n", contentLength);
        Serial.println("Restarting ESP32...");
        delay(1000);
        ESP.restart(); // Restart the ESP32 automatically after a successful update
        break;
    }
  } 
  else {
    Serial.printf("HTTP request failed with error code: %d\n", httpCode);
  }
}
