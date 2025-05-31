const inline uint8_t LED_PINS_OFFSET = 16;
const inline uint8_t RIGHT_PIN_MASK = 3;
const inline uint8_t MIDDLE_PIN_MASK = 28;
const inline uint8_t LEFT_PIN_MASK = 224;

inline void setupLeds(const uint8_t pins) {
  // bits 1 0
  pinMode(LED_PINS_OFFSET | pins & RIGHT_PIN_MASK, OUTPUT);
  // bits 4 3 2
  pinMode(LED_PINS_OFFSET | pins & MIDDLE_PIN_MASK, OUTPUT);
  // bits 7 5 6
  pinMode(LED_PINS_OFFSET | pins & LEFT_PIN_MASK, OUTPUT);
}

inline void writeLeds(const uint8_t pins, const uint8_t configuration) {
  // green led
  digitalWrite(LED_PINS_OFFSET | pins & RIGHT_PIN_MASK, configuration & (1));
  // yellwo led
  digitalWrite(LED_PINS_OFFSET | pins & MIDDLE_PIN_MASK, configuration & (2));
  // red led
  digitalWrite(LED_PINS_OFFSET | pins & LEFT_PIN_MASK, configuration & (4));
}

inline uint8_t generateLedConfiguration(const float iaqi) {
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