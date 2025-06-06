#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>

int setupWiFi() {
  const uint8_t MAX_RETRY = 20;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint8_t it = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (++it == MAX_RETRY) {
      return WiFi.status();
    }
  }
  return WL_CONNECTED;
}

const int shareValues(float temperature, uint8_t humidity, uint16_t analog_read, float iaqi) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, SERVER_PATH);

  http.addHeader(F("Content-Type"), F("text/plain"));

  const int httpResponseCode = http.POST(String(DEVICE_ID) + ";" + temperature + ";" + humidity + ";" + analog_read + ";" + iaqi);

  http.end();
  return httpResponseCode;
}