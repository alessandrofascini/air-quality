#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>

void setup() {
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting");

  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(10000);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Disconnected"); 
    return;
  }
  // WiFi.status() == WL_CONNECTED
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, SERVER_PATH);
  // If you need an HTTP request with a content type: application/json, use the following:
  //http.addHeader("Content-Type", "application/json");
  //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

  // If you need an HTTP request with a content type: text/plain
  http.addHeader("Content-Type", "text/plain");
  int httpResponseCode = http.POST("Hello, World!");
     
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);
        
  // Free resources
  http.end();
}