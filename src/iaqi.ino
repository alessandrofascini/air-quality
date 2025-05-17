float calculate(float ppm, float BPlo, float BPhi, float Ilo, float Ihi) {
  const float delta = (Ihi - Ilo) / (BPhi - BPlo);
  return delta * (ppm - BPlo) + Ilo;
}

float findBreakpointsAndCalculate(const float ppm, const float table[][4], const int n, const float outOfRange) {
  float Ilo = -1., Ihi = 0.;
  float BPlo = 0., BPhi = 0.;
  for (int i = 0; i < n; i++) {
    Ihi = table[i][1];
    if (ppm < Ihi) {
      Ilo = table[i][0];
      BPlo = table[i][2];
      BPhi = table[i][3];
      break;
    }
  }
  if (Ilo == -1) {
    return outOfRange;
  }
  return calculate(ppm, Ilo, Ihi, BPlo, BPhi);
}

float iaqiCO(float ppm) {
  const int n = 7;
  // U.S. EPA indoor AQI
  const float table[n][4] = {
    {0.0, 4.4, 0, 50},
    {4.5, 9.4, 51, 100},
    {9.5, 12.4, 101, 150},
    {12.5, 15.4, 151, 200},
    {15.5, 30.4, 201, 300},
    {30.5, 40.4, 301, 400},
    {40.5, 50.4, 401, 500}
  };
  return findBreakpointsAndCalculate(ppm, table, n, 500) / 5.;
}

float iaqiAlcohol(float ppm) {
    const int n = 5;
  // U.S. EPA indoor AQI
  const float table[n][4] = {
    {   0,  200,   0,  50},
    { 201,  400,  51, 100},
    { 401,  800, 101, 150},
    { 801, 1000, 151, 200},
    {1001, 2000, 201, 300}
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiCO2(float ppm) {
  const int n = 5;
  // U.S. EPA indoor AQI
  const float table[n][4] = {
      { 250,  600,   0,  50},
      { 601, 1000,  51, 100},
      {1001, 1500, 101, 150},
      {1501, 2000, 151, 200},
      {2001, 5000, 201, 300}
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiToluen(float ppm) {
  const int n = 5;
  // U.S. EPA indoor AQI
  const float table[n][4] = {
    { 0.0,  0.1,   0,  50},
    { 0.1,  0.3,  51, 100},
    { 0.3,  1.0, 101, 150},
    { 1.0, 10.0, 151, 200},
    {10.0, 50.0, 201, 300}
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiNH4(float ppm) {
  const int n = 5;
  // U.S. EPA indoor AQI
  const float table[n][4] = {
    { 0.00,  0.25,   0,  50},
    { 0.26,  1.00,  51, 100},
    { 1.10,  5.00, 101, 150},
    { 5.10, 10.00, 151, 200},
    {10.00, 25.00, 201, 300}
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiAceton(float ppm) {
  const int n = 5;
  // U.S. EPA indoor AQI
  const float table[n][4] = {
    {  0,   5,   0,  50},
    {  6,  20,  51, 100},
    { 21, 100, 101, 150},
    {101, 200, 151, 200},
    {201, 500, 201, 300}
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}
