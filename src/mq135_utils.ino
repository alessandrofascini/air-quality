MQUnifiedsensor newMQ135(uint8_t PIN_A0) {
  const float V_REF = 3.3;
  const uint8_t BIT_RESOLUTION = 12;

  MQUnifiedsensor sensor("ESP32", V_REF, BIT_RESOLUTION, PIN_A0, "MQ135");

  sensor.setRegressionMethod(1); //_PPM =  a*ratio^b
  sensor.setRL(20);
  // sensor.setR0(76.81);
  // sensor.setR0(71.6152);
  sensor.setR0(6.9358);
  return sensor;
}
