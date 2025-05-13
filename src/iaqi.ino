float iaqi(float ppm, float Ilo, float Ihi, float BPlo, float BPhi) {
  const float delta = (Ihi - Ilo) / (BPhi - BPlo);
  return delta * (ppm - BPlo) + Ilo;
}

float iaqiCO(float ppm) {
  // find AQI
  float Ilo, Ihi;
  float BPlo, BPhi;

  // todo: into for
  if (ppm <= 4.4) {
    Ilo = 0;
    Ihi = 4.4;
    BPlo = 0;
    BPhi = 50;
  } else if (ppm <= 9.4) {
    Ilo = 4.5;
    Ihi = 9.4;
    BPlo = 51;
    BPhi = 100;
  } else if (ppm <= 12.4) {
    Ilo = 9.5;
    Ihi = 12.4;
    BPlo = 101;
    BPhi = 150;
  } else if (ppm <= 15.4) {
    Ilo = 12.5;
    Ihi = 15.4;
    BPlo = 151;
    BPhi = 200;
  } else if (ppm <= 30.4) {
    Ilo = 15.5;
    Ihi = 30.4;
    BPlo = 201;
    BPhi = 300;
  } else if (ppm <= 40.4) {
    Ilo = 30.5;
    Ihi = 40.4;
    BPlo = 301;
    BPhi = 400;
  } else if (ppm <= 50.4) {
    Ilo = 40.5;
    Ihi = 50.4;
    BPlo = 401;
    BPhi = 500;
  } else {
    // out of table
    return 500.;
  }
  ///
  return iaqi(ppm, Ilo, Ihi, BPlo, BPhi);
}