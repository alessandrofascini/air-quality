class Breakpoint {
private:
  float BPlo, BPhi;
  int Ilo, Ihi;

public:
  Breakpoint(float BPlo, float BPhi, int Ilo, int Ihi)
   : BPlo(BPlo), BPhi(BPhi), Ilo(Ilo), Ihi(Ihi) {}  

  float calculate(const float ppm) const {
    return (Ihi - Ilo) / (BPhi - BPlo)  * (ppm - BPlo) + Ilo;
  }

  float getBPhi() const {
    return BPhi;
  }
};


float findBreakpointsAndCalculate(const float ppm, const Breakpoint* table[], const int n, const float outOfRange) {
  for (int i = 0; i < n; i++) {
    const Breakpoint* bp = table[i];
    if (ppm < bp->getBPhi()) {
      return bp->calculate(ppm);
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
  
  const Breakpoint* table[n] = { &bp0, &bp1, &bp2, &bp3, &bp4, &bp5, &bp6 };

  /* for (int i = 0; i < n; i++) {
    table[i][0] *= 1.5;
    table[i][1] *= 1.5;
  } */
  return findBreakpointsAndCalculate(ppm, table, n, 500) / 5.;
}

float iaqiAlcohol(float ppm) {
  const int n = 5;
  // 
  const Breakpoint bp0(   0,  200,   0,  50);
  const Breakpoint bp1( 201,  400,  51, 100);
  const Breakpoint bp2( 401,  800, 101, 150);
  const Breakpoint bp3( 801, 1000, 151, 200);
  const Breakpoint bp4(1001, 2000, 201, 300);

  const Breakpoint* table[n] = { &bp0, &bp1, &bp2, &bp3, &bp4 };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}

float iaqiCO2(float ppm) {
  const int n = 5;
  // 
  const Breakpoint bp0( 250,  600,   0,  50);
  const Breakpoint bp1( 601, 1000,  51, 100);
  const Breakpoint bp2(1001, 1500, 101, 150);
  const Breakpoint bp3(1501, 2000, 151, 200);
  const Breakpoint bp4(2001, 5000, 201, 300);

  const Breakpoint* table[n] = {
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
  const Breakpoint bp0( 0.0,  0.1,   0,  50);
  const Breakpoint bp1( 0.1,  0.3,  51, 100);
  const Breakpoint bp2( 0.3,  1.0, 101, 150);
  const Breakpoint bp3( 1.0, 10.0, 151, 200);
  const Breakpoint bp4(10.0, 50.0, 201, 300);

  const Breakpoint* table[n] = {
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
  const Breakpoint bp0( 0.00,  0.25,   0,  50);
  const Breakpoint bp1( 0.26,  1.00,  51, 100);
  const Breakpoint bp2( 1.10,  5.00, 101, 150);
  const Breakpoint bp3( 5.10, 10.00, 151, 200);
  const Breakpoint bp4(10.00, 25.00, 201, 300);

  const Breakpoint* table[n] = {
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
  const Breakpoint bp0(  0,   5,   0,  50);
  const Breakpoint bp1(  6,  20,  51, 100);
  const Breakpoint bp2( 21, 100, 101, 150);
  const Breakpoint bp3(101, 200, 151, 200);
  const Breakpoint bp4(201, 500, 201, 300);

  const Breakpoint* table[n] = {
    &bp0,
    &bp1,
    &bp2,
    &bp3,
    &bp4
  };
  ///
  return findBreakpointsAndCalculate(ppm, table, n, 300) / 3.;
}
