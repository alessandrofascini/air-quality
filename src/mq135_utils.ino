/// Parameters to model temperature and humidity dependence
#define CORA 0.00035
#define CORB 0.02718
#define CORC 1.39538
#define CORD 0.0018

MQUnifiedsensor newMQ135(uint8_t PIN_A0) {
  const float V_REF = 3.3;
  const uint8_t BIT_RESOLUTION = 12;
  MQUnifiedsensor sensor("ESP32", V_REF, BIT_RESOLUTION, PIN_A0, "MQ135");
  
  sensor.setRegressionMethod(1); //_PPM =  a*ratio^b
  sensor.setRL(20);
  sensor.setR0(76.81);
  return sensor;
}

/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity
@param[in] t  The ambient air temperature
@param[in] h  The relative humidity
@return The calculated correction factor
*/
/**************************************************************************/
float getCorrectionFactor(float t, uint8_t h) {
  return CORA * t * t - CORB * t + CORC - (h-33.)*CORD;
}

float ppmPrepare(MQUnifiedsensor *sensor, uint16_t analog, float t, uint8_t h) {
  sensor->setADC(analog);
  return getCorrectionFactor(t, h);
}

float ppmReader(MQUnifiedsensor *sensor, const float a, const float b, const float cFactor) {
  sensor->setA(a);
  sensor->setB(b);
  return sensor->readSensor(false, cFactor);
}

float ppmCO(MQUnifiedsensor *sensor, const float cFactor) {
  return ppmReader(sensor, 605.18, -3.937, cFactor);
}

float ppmAlcohol(MQUnifiedsensor *sensor, const float cFactor) {
  return ppmReader(sensor, 77.255, -3.18, cFactor);
}

float ppmCO2(MQUnifiedsensor *sensor, const float cFactor) {
  /*
    Motivation:
    We have added 400 PPM because when the library is calibrated it assumes the current state of the
    air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
    https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */
  return ppmReader(sensor, 110.47, -2.862, cFactor) + 400.;
}

float ppmToluen(MQUnifiedsensor *sensor, const float cFactor) {
  return ppmReader(sensor, 44.947, -3.445, cFactor);
}

float ppmNH4(MQUnifiedsensor *sensor, const float cFactor) {
  return ppmReader(sensor, 102.2, -2.473, cFactor);
}

float ppmAceton(MQUnifiedsensor *sensor, const float cFactor) {
  return ppmReader(sensor, 34.668, -3.369, cFactor);
}
