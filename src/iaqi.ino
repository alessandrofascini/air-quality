struct Breakpoint {
  float BPlo, BPhi;
  uint Ilo, Ihi;
};

inline float findBreakpointsAndCalculate(float ppm, struct Breakpoint* table[], const int n, float outOfRange) {
  for (int i = 0; i < n; i++) {
    const Breakpoint* bp = table[i];
    if (ppm < bp->BPhi) {
      return (bp->Ihi - bp->Ilo) / (bp->BPhi - bp->BPlo)  * (ppm - bp->BPlo) + bp->Ilo;
    }
  }
  return outOfRange;
}

float iaqiCO(float ppm) {
  const int n = 7;
  
  // U.S. EPA indoor AQI
  Breakpoint bp0(0.0, 4.4, 0, 50);
  Breakpoint bp1(4.5, 9.4, 51, 100);
  Breakpoint bp2(9.5, 12.4, 101, 150);
  Breakpoint bp3(12.5, 15.4, 151, 200);
  Breakpoint bp4(15.5, 30.4, 201, 300);
  Breakpoint bp5(30.5, 50.4, 301, 400);
  Breakpoint bp6(50.5, 604.4, 401, 500);
  
  Breakpoint* table[n] = { &bp0, &bp1, &bp2, &bp3, &bp4, &bp5, &bp6 };

  return findBreakpointsAndCalculate(ppm, table, n, 500) / 5.;
}

float iaqiAlcohol(float ppm) {
  const int n = 5;
  // 
  Breakpoint bp0(   0,  200,   0,  50);
  Breakpoint bp1( 201,  400,  51, 100);
  Breakpoint bp2( 401,  800, 101, 150);
  Breakpoint bp3( 801, 1000, 151, 200);
  Breakpoint bp4(1001, 2000, 201, 300);

  Breakpoint* table[n] = { &bp0, &bp1, &bp2, &bp3, &bp4 };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiCO2(float ppm) {
  const int n = 5;
  // 
  Breakpoint bp0( 250,  600,   0,  50);
  Breakpoint bp1( 601, 1000,  51, 100);
  Breakpoint bp2(1001, 1500, 101, 150);
  Breakpoint bp3(1501, 2000, 151, 200);
  Breakpoint bp4(2001, 5000, 201, 300);

  Breakpoint* table[n] = {
    &bp0,
    &bp1,
    &bp2,
    &bp3,
    &bp4
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiToluen(float ppm) {
  const int n = 5;
  // 
  Breakpoint bp0( 0.0,  0.1,   0,  50);
  Breakpoint bp1( 0.1,  0.3,  51, 100);
  Breakpoint bp2( 0.3,  1.0, 101, 150);
  Breakpoint bp3( 1.0, 10.0, 151, 200);
  Breakpoint bp4(10.0, 50.0, 201, 300);

  Breakpoint* table[n] = {
    &bp0,
    &bp1,
    &bp2,
    &bp3,
    &bp4
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiNH4(float ppm) {
  const int n = 5;
  // 
  Breakpoint bp0( 0.00,  0.25,   0,  50);
  Breakpoint bp1( 0.26,  1.00,  51, 100);
  Breakpoint bp2( 1.10,  5.00, 101, 150);
  Breakpoint bp3( 5.10, 10.00, 151, 200);
  Breakpoint bp4(10.00, 25.00, 201, 300);

  Breakpoint* table[n] = {
    &bp0,
    &bp1,
    &bp2,
    &bp3,
    &bp4
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiAceton(float ppm) {
  const int n = 5;
  // 
  Breakpoint bp0(  0,   5,   0,  50);
  Breakpoint bp1(  6,  20,  51, 100);
  Breakpoint bp2( 21, 100, 101, 150);
  Breakpoint bp3(101, 200, 151, 200);
  Breakpoint bp4(201, 500, 201, 300);

  Breakpoint* table[n] = {
    &bp0,
    &bp1,
    &bp2,
    &bp3,
    &bp4
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqIndex(float t, uint8_t h, uint16_t value) {
  Serial.println(F("=> PPM <=")); // todo: better here

  const float correctionFactor = ppmPrepare(&MQ135, value, t, h);
  Serial.print(F("Correction Factor: ")); Serial.println(correctionFactor);
  
  const float CO = ppmCO(&MQ135, correctionFactor);
  float iaqi = iaqiCO(CO);
  Serial.print(F("CO: ")); Serial.print(CO, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi, 3);

  const float Alcohol = ppmAlcohol(&MQ135, correctionFactor);
  const float iaqi_alcohol = iaqiAlcohol(Alcohol);
  iaqi = max(iaqi, iaqi_alcohol);
  Serial.print(F("Alcohol: ")); Serial.print(Alcohol, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_alcohol, 3);

  const float CO2 = ppmCO2(&MQ135, correctionFactor);
  const float iaqi_CO2 = iaqiCO2(CO2);
  iaqi = max(iaqi, iaqi_CO2);
  Serial.print(F("CO2: ")); Serial.print(CO2, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_CO2, 3);

  const float Toluen = ppmToluen(&MQ135, correctionFactor);
  const float iaqi_toluen = iaqiToluen(Toluen);
  iaqi = max(iaqi, iaqi_toluen);
  Serial.print(F("Toluen: ")); Serial.print(Toluen, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_toluen, 3);
  
  const float NH4 = ppmNH4(&MQ135, correctionFactor);
  const float iaqi_NH4 = iaqiNH4(NH4);
  iaqi = max(iaqi, iaqi_NH4);
  Serial.print(F("NH4: ")); Serial.print(NH4, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_NH4, 3); 

  const float Aceton = ppmAceton(&MQ135, correctionFactor);
  const float iaqi_aceton = iaqiAceton(Aceton);
  iaqi = max(iaqi, iaqi_aceton);
  Serial.print(F("Aceton: ")); Serial.print(Aceton, 3); Serial.print(F(" - IAQI: ")); Serial.println(iaqi_aceton, 3);

  iaqi = 100 - iaqi;
  Serial.print(F("=> IAQI = ")); Serial.print(iaqi, 3); Serial.println(F(" <="));
  return iaqi;
}