#include <MQUnifiedsensor.h>
#include <DHT.h>

// constants
const uint8_t DHT11_D0 = 23;
const uint8_t MQ135_A0 = 34;

const float RATIO_CLEAN_AIR = 3.6;
const unsigned long MAX_READINGS = 30;

// globals
MQUnifiedsensor newMQ135();
MQUnifiedsensor MQ135 = newMQ135();

DHT dht11(DHT11_D0, DHT11);

void setup() {
  Serial.begin(115200);

  delay(2000);

  setupWiFi();
  MQ135.init();
  dht11.begin();

  const float temperature = dht11.readTemperature();
  Serial.print(F("T = ")); Serial.print(temperature); Serial.println(F(" °C"));

  const float humidity = dht11.readHumidity();
  Serial.print(F("RH = ")); Serial.print(humidity); Serial.println(F(" %"));
}

void loop() {
  double sum = 0.0;
  int counter = 0;
  float measurements[MAX_READINGS];
  for(int i = 0; i < MAX_READINGS; i++) {
    MQ135.update();
    const float R0 = MQ135.calibrate(RATIO_CLEAN_AIR);
    if(!isnan(R0) && !isinf(R0)) {
      sum += R0;
      counter++;
    } 
    measurements[i] = R0;
    delay(500);
  }
 
  const float temperature = dht11.readTemperature();
  Serial.print(F("T = ")); Serial.print(temperature, 2); Serial.println(F(" °C"));

  const float humidity = dht11.readHumidity();
  Serial.print(F("RH = ")); Serial.print(humidity, 0); Serial.println(F(" %"));

  if(counter > 0) {
    const double R0 = sum / (double)counter;
    Serial.print(F("R0 = ")); Serial.print(R0, 4); Serial.println((" KΩ"));
  }

  const int response = shareValues(temperature, humidity, measurements, MAX_READINGS);
  Serial.print("HTTP Response: "); Serial.println(response);
  delay(2000);
}

MQUnifiedsensor newMQ135() {
  MQUnifiedsensor sensor("ESP32", 3.3, 12, MQ135_A0, "MQ135");
  sensor.setRegressionMethod(1); //_PPM =  a*ratio^b
  sensor.setRL(20);
  return sensor;
}
