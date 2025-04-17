#include "DHT.h"

#define DHT_TYPE DHT11

#define DHT_PIN 18

DHT dht(DHT_PIN, DHT_TYPE);       

void setup() {
  Serial.begin(115200);

  pinMode(DHT_PIN, INPUT);
  dht.begin(); 
}

void loop() {
  int temperature = (int)dht.readTemperature();
  int humidity = (int)dht.readHumidity(); 

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("an error occurs during the reading");
  } else {
    // Calcolare il punto di rugiada
    float dewPoint = temperature - ((100 - humidity) / 5);

    // Calcolare l'indice di calore
    float heatIndex = -8.7847 + 1.6114 * temperature + 2.3385 * humidity - 0.1461 * temperature * humidity - 0.0123 * temperature * temperature - 0.0164 * humidity * humidity + 0.0022 * temperature * temperature * humidity + 0.0008 * temperature * humidity * humidity - 0.0001 * temperature * temperature * humidity * humidity;

    // Visualizzare le informazioni
    Serial.print("Temperature: [");
    Serial.print(temperature - 2);
    Serial.print(", ");
    Serial.print(temperature + 2);
    Serial.print("]");
    Serial.print(" C | Humidity: ");
    Serial.print(humidity);
    Serial.print(" % | Dew Point: ");
    Serial.print(dewPoint);
    Serial.print(" C | Heat Index: ");
    Serial.print(heatIndex);
    Serial.println(" C");

    // Segnali di avviso
    if (dewPoint > temperature) {
      Serial.println("Warning: Condensation risk!");
    }
    if (heatIndex > 30) {
      Serial.println("Warning: High heat index! Take precautions.");
    }
  }

  delay(3000);
}