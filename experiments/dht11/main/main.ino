#include "DHT.h"

#define DHT_PIN 18

DHT dht(DHT_PIN, DHT11);       

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("the sensor is waking up....");
  dht.begin(); 

  delay(2000);
}

void loop() {
  const float temperature = dht.readTemperature(false);
  const float humidity = dht.readHumidity(); 

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("an error occurs during the reading");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperature,1);
    Serial.print(" °C ±2; Humidity: ");
    Serial.print(humidity,0);
    Serial.print(" % ±5; ");
    
    const float heatIndex = dht.computeHeatIndex(temperature, humidity, false);
    Serial.print("HeatIndex: ");
    Serial.print(heatIndex);
    Serial.println(" °C");
  }

  delay(2000);
}