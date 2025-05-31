const inline float V_REF = 3.3;
const inline uint8_t BIT_RESOLUTION = 12;
const inline uint8_t RL = 20;
// const inline float R0 = 76.81;
// const inline float R0 = 71.6152;
// const inline float R0 = 6.9358;
// const inline float R0 = 5.60617;
const inline float R0 = 5.36755;

MQUnifiedsensor newMQ135(uint8_t PIN_A0) {
  MQUnifiedsensor sensor("", V_REF, BIT_RESOLUTION, PIN_A0, "");
  sensor.setRegressionMethod(1); //_PPM =  a*ratio^b
  sensor.setRL(RL);
  sensor.setR0(R0);
  return sensor;
}
