#include "led.cpp"
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <TaskScheduler.h>
#include "RunningAverage.h"

/// CONSTANTS
const uint8_t GREEN_LED = 18;
const uint8_t YELLOW_LED = 19;
const uint8_t RED_LED = 21;

const uint8_t MQ135_A0 = 34;
// const uint8_t MQ135_D0 = 36;
const uint8_t DHT_PIN = 23;

/// GLOBALS
MQUnifiedsensor newMQ135(uint8_t PIN_A0); 

Scheduler ts;
MQUnifiedsensor MQ135 = newMQ135(MQ135_A0);
DHT dht(DHT_PIN, DHT11);
GYRLed leds(GREEN_LED, YELLOW_LED, RED_LED);

RunningAverage temperature(15);
RunningAverage humidity(15);
RunningAverage gas(30);


/// FUNCTIONS PROTOTYPE
void readTemperatureAndHumidity();
void readGas();
void output();

/// TASKS

Task tTempHum(2 * TASK_SECOND,
              TASK_FOREVER,
              readTemperatureAndHumidity,
              &ts,
              true
);

Task tGas(TASK_SECOND,
          TASK_FOREVER,
          readGas,
          &ts,
          true);


Task tOutput(30 * TASK_SECOND,
             TASK_FOREVER,
             output,
             &ts,
             true
);


/// SETUP
void setup() {
  Serial.begin(115200);

  /// setup led
  leds.setup();

  // setupWifi
  setupWiFi();

  // MQ135
  MQ135.init();
  // pinMode(MQ135_D0, INPUT);

  // DHT11
  dht.begin();
}

/// LOOP
void loop() {
  ts.execute();
}

/// FUNCTIONS

void readTemperatureAndHumidity() {
  const float t = dht.readTemperature() - 1.;
  const uint8_t h = (uint8_t)dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println(F("failed to read from DHT sensor!"));
  } else {
    // Serial.print(F("temperature: ")); Serial.println(t);
    temperature.addValue(t);

    // Serial.print(F("humidity: ")); Serial.println(h);
    humidity.addValue(h);
  }
}


void readGas() {
  const uint16_t value = analogRead(MQ135_A0);
 // Serial.print(F("analog read: ")); Serial.println(value);
  gas.addValue(value);
}

void output() {
  const float t = temperature.getAverage();
  const uint8_t h = humidity.getAverage();
  const uint16_t value = gas.getAverage();

  Serial.println(F("PPMs:"));

  const float CO = ppmCO(&MQ135, t, h, value);
  Serial.print(F("CO: ")); Serial.println(CO, 3);
  Serial.print(F("IAQI_CO: ")); Serial.println(iaqiCO(CO), 3);

  const float Alcohol = ppmAlcohol(&MQ135, t, h, value);
  Serial.print(F("Alcohol: ")); Serial.println(Alcohol, 3);

  const float CO2 = ppmCO2(&MQ135, t, h, value);
  Serial.print(F("CO2: ")); Serial.println(CO2, 3);

  const float Toluen = ppmToluen(&MQ135, t, h, value);
  Serial.print(F("Toluen: ")); Serial.println(Toluen, 3);
  
  const float NH4 = ppmNH4(&MQ135, t, h, value);
  Serial.print(F("NH4: ")); Serial.println(NH4, 3);

  const float Aceton = ppmAceton(&MQ135, t, h, value);
  Serial.print(F("Aceton: ")); Serial.println(Aceton, 3);

  const float IAQ = random(0, 101);
  /// generate configuration
  uint8_t conf = 0x0;

  const uint8_t HIGH_GREEN = 1;
  const uint8_t HIGH_YELLOW = 2;
  const uint8_t HIGH_RED = 4;

  if (60 <= IAQ) {
    conf |= HIGH_GREEN;
  }
  if (20 <= IAQ && IAQ < 80) {
    conf |= HIGH_YELLOW;
  }
  if (IAQ < 40) {
    conf |= HIGH_RED;
  }
  ////

  leds.show(conf);
}
