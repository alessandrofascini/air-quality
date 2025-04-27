#include "led.cpp"
// #include <MQ135.h>
#include <DHT.h>
#include <TaskScheduler.h>

/// CONSTANTS
const uint8_t GREEN_LED = 18;
const uint8_t YELLOW_LED = 19;
const uint8_t RED_LED = 21;

const uint8_t MQ135_A0 = 35;
const uint8_t MQ135_D0 = 36;
const uint8_t DHT_PIN = 23;

/// GLOBALS
Scheduler ts;
// MQ135 mq135(MQ135_PIN, 76.63, 20.);
DHT dht(DHT_PIN, DHT11);
GYRLed leds(GREEN_LED, YELLOW_LED, RED_LED);

float temperature;
uint8_t humidity;
float ppm;

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

Task tGas(5 * TASK_SECOND,
          TASK_FOREVER,
          readGas,
          &ts,
          true);


Task tOutput(10 * TASK_SECOND,
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
  pinMode(MQ135_D0, INPUT);

  // DHT11
  dht.begin();
}

/// LOOP
void loop() {
  ts.execute();
}

/// FUNCTIONS

void readTemperatureAndHumidity() {
  const float t = dht.readTemperature();
  const uint8_t h = (uint8_t)dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println(F("failed to read from DHT sensor!"));
  } else {
    temperature = t;
    Serial.print(F("temperature: "));
    Serial.println(t);

    humidity = h;
    Serial.print(F("humidity: "));
    Serial.println(h);
  }
}


void readGas() {
  ppm = analogRead(MQ135_A0);
  const uint8_t isGas = digitalRead(MQ135_D0);

  Serial.print(F("mq135: "));
  Serial.print(ppm);
  if(isGas) {
    Serial.println(F(" no"));
  } else {
    Serial.println(F(" yes"));
  }
}

void output() {
  /// IAQ = 100 − (w₁⋅G + w₂⋅∣H − θ∣ + w₃⋅∣T − λ∣)
  // const uint8_t value = random(0, 101);
  // here latest value readed
  float IAQ = ppm * .05;
  IAQ += .5 * abs(humidity - 50);
  IAQ += abs(temperature - 22);
  IAQ = 100 - IAQ;

  Serial.println("----");
  Serial.println(temperature);
  Serial.println(humidity);
  Serial.println(ppm);
  Serial.print(F("IAQ: "));
  Serial.println(IAQ);

  /// generate configuration
  uint8_t conf = 0x0;
  if (60 <= IAQ) {
    conf |= 1;
  }
  if (20 <= IAQ && IAQ < 80) {
    conf |= 2;
  }
  if (IAQ < 40) {
    conf |= 4;
  }
  ////

  leds.show(conf);
}
