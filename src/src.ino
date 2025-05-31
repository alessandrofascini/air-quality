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
const uint8_t MQ135_D0 = 22;
const uint8_t DHT_PIN = 23;

/// FUNCTIONS PROTOTYPE
MQUnifiedsensor newMQ135(uint8_t PIN_A0);
void readTemperatureAndHumidity();
void readGas();
void readGasImmediate();
void startTaskOutput();
void output();

Scheduler ts;
MQUnifiedsensor MQ135 = newMQ135(MQ135_A0);
DHT dht(DHT_PIN, DHT11);
GYRLed leds(GREEN_LED, YELLOW_LED, RED_LED);

RunningAverage temperature(15);
RunningAverage humidity(15);
RunningAverage gas(40);

/// TASKS

Task tTempHum(2 * TASK_SECOND,
              TASK_FOREVER,
              readTemperatureAndHumidity,
              &ts,
              true);

Task tGas(TASK_SECOND,
          TASK_FOREVER,
          readGas,
          &ts,
          true);

Task tGasImmediate(TASK_IMMEDIATE,
                   TASK_FOREVER,
                   readGasImmediate,
                   &ts,
                   false);

Task tOutput(30 * TASK_SECOND,
             TASK_FOREVER,
             output,
             &ts,
             true);

/// INTERRUPTS
volatile unsigned long last_call = 0;
volatile bool readGasNow = false;
void IRAM_ATTR isr() {
  const unsigned long now = millis();
  if (now - last_call > 800) {
    last_call = now;
    readGasNow = true;
  }
}

/// SETUP
void setup() {
  Serial.begin(115200);
  delay(2000);

  const String SETUP = F("[SETUP]");
  const String DONE = F("done");

  Serial.print(SETUP);
  Serial.println(F(" begin"));

  /// setup led
  Serial.print(SETUP);
  Serial.print(F(" LED: "));
  leds.setup();
  Serial.println(DONE);

  // setupWifi
  Serial.print(SETUP);
  Serial.print(F(" WI-FI: "));
  setupWiFi();
  Serial.println(DONE);

  // MQ135
  Serial.print(SETUP);
  Serial.print(F(" MQ135: "));
  MQ135.init();

  if (digitalPinToInterrupt(MQ135_D0) == -1) {
    Serial.print(F("\n | cannot attach interrupt on pin "));
    Serial.print(MQ135_D0);
  } else {
    pinMode(MQ135_D0, INPUT_PULLUP);
    attachInterrupt(MQ135_D0, isr, FALLING);
    Serial.print(F("\n | attached interrupt on pin "));
    Serial.print(MQ135_D0);
  }
  Serial.print(F("\n | "));
  Serial.println(DONE);

  // DHT11
  Serial.print(SETUP);
  Serial.print(F(" DHT11: "));
  dht.begin();
  Serial.println(DONE);

  Serial.print(SETUP);
  Serial.println(F(" finished\n"));

  tOutput.enableDelayed(30 * TASK_SECOND);
}

/// LOOP
void loop() {
  if (readGasNow) {
    readGasNow = false;
    tGasImmediate.enable();
  }
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

inline void mq135AnalogRead() {
  const uint16_t value = analogRead(MQ135_A0);
  // Serial.print(F("ar: ")); Serial.println(value);
  gas.addValue(value);
}

void readGas() {
  mq135AnalogRead();
}

void readGasImmediate() {
  //Serial.println(F("gas readed from interrupt!"));
  mq135AnalogRead();
  tGasImmediate.disable();
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

void output() {
  const float t = temperature.getAverage();
  const uint8_t h = humidity.getAverage();
  const uint16_t analog_read = gas.getAverage();

  Serial.println(F("=> DATA <="));
  Serial.print(F("T: "));
  Serial.print(t);
  Serial.println(F(" °C"));
  Serial.print(F("H: "));
  Serial.print(h);
  Serial.println(F(" %"));
  Serial.print(F("V: "));
  Serial.print(analog_read);
  Serial.println();

  const float iaqi = iaqIndex(t, h, analog_read);

  const uint8_t conf = generateLedConfiguration(iaqi);
  leds.show(conf);

  ///
  const int16_t responseCode = shareValues(t, h, analog_read, iaqi);
  Serial.print(F("HTTP POST: "));
  if (responseCode == 200) {
    Serial.println(F("OK"));
  } else {
    Serial.println(responseCode);
  }
  Serial.println();
}
