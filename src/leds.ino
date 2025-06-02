const inline uint8_t LED_PINS_OFFSET = 16;

const inline uint8_t RIGHT_PIN_MASK = 3;
const inline uint8_t MIDDLE_PIN_MASK = 28;
const inline uint8_t LEFT_PIN_MASK = 224;

const inline uint8_t RIGHT_PIN_SHIFT = 0;
const inline uint8_t MIDDLE_PIN_SHIFT = 2;
const inline uint8_t LEFT_PIN_SHIFT = 5;

const inline uint8_t HIGH_GREEN = 1;
const inline uint8_t HIGH_YELLOW = 2;
const inline uint8_t HIGH_RED = 4;

inline uint8_t unmaskPin(const uint8_t pins, const uint8_t mask, const uint8_t shift, const uint8_t offset) {
  return offset | ((pins & mask) >> shift);
}

inline void setupLeds(const uint8_t pins) {
  // bits 1 0
  const uint8_t green_pin = unmaskPin(pins, RIGHT_PIN_MASK, RIGHT_PIN_SHIFT, LED_PINS_OFFSET);
  pinMode(green_pin, OUTPUT);

  // bits 4 3 2
  const uint8_t yellow_pin = unmaskPin(pins, MIDDLE_PIN_MASK, MIDDLE_PIN_SHIFT, LED_PINS_OFFSET);
  pinMode(yellow_pin, OUTPUT);
  
  // bits 7 5 6
  const uint8_t red_pin = unmaskPin(pins, LEFT_PIN_MASK, LEFT_PIN_SHIFT, LED_PINS_OFFSET);
  pinMode(red_pin, OUTPUT);
}

inline void writeLeds(const uint8_t pins, const uint8_t conf) {
  // green led
  const uint8_t green_pin = unmaskPin(pins, RIGHT_PIN_MASK, RIGHT_PIN_SHIFT, LED_PINS_OFFSET);
  digitalWrite(green_pin, conf & HIGH_GREEN);

  // yellow led
  const uint8_t yellow_pin = unmaskPin(pins, MIDDLE_PIN_MASK, MIDDLE_PIN_SHIFT, LED_PINS_OFFSET);
  digitalWrite(yellow_pin, conf & HIGH_YELLOW);

  // red led
  const uint8_t red_pin = unmaskPin(pins, LEFT_PIN_MASK, LEFT_PIN_SHIFT, LED_PINS_OFFSET);
  digitalWrite(red_pin, conf & HIGH_RED);
}

inline uint8_t generateLedConfiguration(const float iaqi) {
  uint8_t conf = 0x0;

  if (iaqi < 40) {
    conf |= HIGH_RED;
  }
  if (20 <= iaqi && iaqi < 80) {
    conf |= HIGH_YELLOW;
  }
  if (60 <= iaqi) {
    conf |= HIGH_GREEN;
  }

  return conf;
}