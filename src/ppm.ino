/// Parameters to model temperature and humidity dependence
const inline float CORA = 0.00035;
const inline float CORB = 0.02718;
const inline float CORC = 1.39538;
const inline float CORD = 0.0018;

/**************************************************************************/
/*!
@brief  Get the correction factor to correct for temperature and humidity
@param[in] t  The ambient air temperature
@param[in] h  The relative humidity
@return The calculated correction factor
*/
/**************************************************************************/
inline float getCorrectionFactor(float t, uint8_t h) {
  return CORA * t * t - CORB * t + CORC - (h-33.) * CORD;
}

inline float ppmPrepare(MQUnifiedsensor *sensor, uint16_t analog, float t, uint8_t h) {
  sensor->setADC(analog);
  return getCorrectionFactor(t, h);
}

inline float readPPM(MQUnifiedsensor *sensor, const float a, const float b, const float cFactor) {
  sensor->setA(a);
  sensor->setB(b);
  return sensor->readSensor(false, cFactor);
}

float ppmCO(MQUnifiedsensor *sensor, const float cFactor) {
  return readPPM(sensor, 605.18, -3.937, cFactor);
}

float ppmAlcohol(MQUnifiedsensor *sensor, const float cFactor) {
  return readPPM(sensor, 77.255, -3.18, cFactor);
}

float ppmCO2(MQUnifiedsensor *sensor, const float cFactor) {
  /*
    Motivation:
    We have added 400 PPM because when the library is calibrated it assumes the current state of the
    air as 0 PPM, and it is considered today that the CO2 present in the atmosphere is around 400 PPM.
    https://www.lavanguardia.com/natural/20190514/462242832581/concentracion-dioxido-cabono-co2-atmosfera-bate-record-historia-humanidad.html
  */
  return readPPM(sensor, 110.47, -2.862, cFactor) + 400.;
}

float ppmToluen(MQUnifiedsensor *sensor, const float cFactor) {
  return readPPM(sensor, 44.947, -3.445, cFactor);
}

float ppmNH4(MQUnifiedsensor *sensor, const float cFactor) {
  return readPPM(sensor, 102.2, -2.473, cFactor);
}

float ppmAceton(MQUnifiedsensor *sensor, const float cFactor) {
  return readPPM(sensor, 34.668, -3.369, cFactor);
}
