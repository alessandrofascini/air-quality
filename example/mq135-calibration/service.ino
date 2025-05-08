#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <format>
#include <string>

int setupWiFi()
{
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

int shareValues(float temperature, float humidity, float r[], int r_len)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, SERVER_PATH);

  http.addHeader(F("Content-Type"), F("text/plain"));

  String message = String(temperature) + ";" + humidity + ";";
  for (int i = 0; i < r_len; i++) {
    message += String(r[i]) + ";";
  }
  int httpResponseCode = http.POST(message);

  // Free resources
  http.end();

  return httpResponseCode;
}