#include "led.cpp"
#include <MQUnifiedsensor.h>
#include <DHT.h>
#include <TaskScheduler.h>
#include <RunningAverage.h>

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
void startTaskOutput();
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
  true
);

Task tStartOutput(TASK_IMMEDIATE,
  1,
  startTaskOutput,
  &ts,
  false
);

Task tOutput(30 * TASK_SECOND,
  TASK_FOREVER,
  output,
  &ts,
  true
);

/// SETUP
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("[SETUP] begin");

  const String DONE = "DONE";

  /// setup led
  Serial.print("[SETUP] LED: ");
  leds.setup();
  Serial.println(DONE);

  // setupWifi
  Serial.print("[SETUP] WI-FI: ");
  setupWiFi();
  Serial.println(DONE);

  // MQ135
  Serial.print("[SETUP] MQ135: ");
  MQ135.init();
  // pinMode(MQ135_D0, INPUT);
  Serial.println(DONE);

  // DHT11
  Serial.print("[SETUP] DHT11: ");
  dht.begin();
  Serial.println(DONE);

  Serial.println("[SETUP] finished\n");

  tOutput.enableDelayed(30 * TASK_SECOND);
}

/// LOOP
void loop() {
  ts.execute();
}

/// FUNCTIONS

void readTemperatureAndHumidity() {
  // corrector value
  const float t = dht.readTemperature() - 1.;
  const uint8_t h = (uint8_t)dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println(F("[ERR] failed to read from DHT sensor!"));
  } else {
    // Serial.print(F("temperature: ")); Serial.println(t);
    temperature.addValue(t);
    // Serial.print(F("humidity: ")); Serial.println(h);
    humidity.addValue(h);
  }
}

void readGas() {
  const uint16_t value = analogRead(MQ135_A0);
  // Serial.print(F("ar: ")); Serial.println(value);
  gas.addValue(value);
}

float percentIAQI(float t, uint8_t h, uint16_t value) {
  Serial.println(F("=> PPM <=")); // todo: better here

  const float correctionFactor = ppmPrepare(&MQ135, value, t, h);
  Serial.print(F("Correction Factor: ")); Serial.println(correctionFactor);
  
  const float CO = ppmCO(&MQ135, correctionFactor);
  float iaqi = iaqiCO(CO);
  Serial.print(F("CO: ")); Serial.print(CO, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi, 3);

  const float Alcohol = ppmAlcohol(&MQ135, correctionFactor);
  const float iaqi_alcohol = iaqiAlcohol(Alcohol);
  iaqi = max(iaqi, iaqi_alcohol);
  Serial.print(F("Alcohol: ")); Serial.print(Alcohol, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_alcohol, 3);

  const float CO2 = ppmCO2(&MQ135, correctionFactor);
  const float iaqi_CO2 = iaqiCO2(CO2);
  iaqi = max(iaqi, iaqi_CO2);
  Serial.print(F("CO2: ")); Serial.print(CO2, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_CO2, 3);

  const float Toluen = ppmToluen(&MQ135, correctionFactor);
  const float iaqi_toluen = iaqiToluen(Toluen);
  iaqi = max(iaqi, iaqi_toluen);
  Serial.print(F("Toluen: ")); Serial.print(Toluen, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_toluen, 3);
  
  const float NH4 = ppmNH4(&MQ135, correctionFactor);
  const float iaqi_NH4 = iaqiNH4(NH4);
  iaqi = max(iaqi, iaqi_NH4);
  Serial.print(F("NH4: ")); Serial.print(NH4, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_NH4, 3); 

  const float Aceton = ppmAceton(&MQ135, correctionFactor);
  const float iaqi_aceton = iaqiAceton(Aceton);
  iaqi = max(iaqi, iaqi_aceton);
  Serial.print(F("Aceton: ")); Serial.print(Aceton, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_aceton, 3);

  iaqi = 100 - iaqi;
  Serial.print(F("=> IAQI = ")); Serial.print(iaqi, 3); Serial.println(F(" <="));
  return iaqi;
}

uint8_t generateLedConfiguration(float iaqi) {
  const uint8_t HIGH_GREEN = 1;
  const uint8_t HIGH_YELLOW = 2;
  const uint8_t HIGH_RED = 4;

  uint8_t conf = 0x0;

  if (20 <= iaqi && iaqi < 80) {
    conf |= HIGH_YELLOW;
  }
    if (iaqi < 40) {
    conf |= HIGH_RED;
  }
  if (60 <= iaqi) {
    conf |= HIGH_GREEN;
  }

  return conf;
}

void startTaskOutput() {
  tOutput.enable();
}

void output() {
  const float t = temperature.getAverage();
  const uint8_t h = humidity.getAverage();
  const uint16_t analog_read = gas.getAverage();

  Serial.println(F("=> DATA <="));
  Serial.print(F("T: ")); Serial.print(t); Serial.println(F(" °C"));
  Serial.print(F("H: ")); Serial.print(h); Serial.println(F(" %"));
  Serial.print(F("V: ")); Serial.print(analog_read); Serial.println();

  const float iaqi = percentIAQI(t, h, analog_read);

  const uint8_t conf = generateLedConfiguration(iaqi);
  leds.show(conf);

  /// 
  const int16_t responseCode = shareValues(t, h, analog_read);
  Serial.print(F("HTTP POST: "));
  if (responseCode == 200) {
    Serial.println(F("OK"));
  } else {
    Serial.println(responseCode);
  }
  Serial.println();
}
