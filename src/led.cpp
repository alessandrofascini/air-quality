#include <Arduino.h>

class GYRLed
{
public:
  GYRLed(uint8_t green, uint8_t yellow, uint8_t red)
  {
    this->leds[0] = green;
    this->leds[1] = yellow;
    this->leds[2] = red;
  }

  void setup()
  {
    for(int i = 0; i < 3; i++) {
      pinMode(this->leds[i], OUTPUT);
    }
  }

  void show(uint8_t configuration) {
    for(int i = 0; i < 3; i++) {
      digitalWrite(this->leds[i], configuration & (1 << i));
    }
  }

private:
  uint8_t leds[3];
};