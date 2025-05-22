#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <format>
#include <string>

int setupWiFi() {
  const uint8_t retry = 20;

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint8_t it = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if (++it == retry)
    {
      return WiFi.status();
    }
  }
  return WL_CONNECTED;
}

int shareValues(float temperature, uint8_t humidity, uint16_t analog_read, float iaqi) {
  WiFiClient client;
  HTTPClient http;

  http.begin(client, SERVER_PATH);

  http.addHeader(F("Content-Type"), F("text/plain"));

  int httpResponseCode = http.POST(String(temperature) + ";" + humidity + ";" + analog_read + ";" + iaqi);
  http.end();
  return httpResponseCode;
}