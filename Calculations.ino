const uint32_t JulianDateNoonJan_1_2000 = 2451545; //https://en.wikipedia.org/wiki/Julian_day

float RADIANS(float degrees) {
  return degrees * PI / 180.0;
}

float DEGREES(float radians) {
  return radians * 180 / PI;
}

float COS(float val) {
  return cos(val);
}

float ACOS(float val) {
  return acos(val);
}

float SIN(float val) {
  return sin(val);
}

float ASIN(float val) {
  return asin(val);
}

float TAN(float val) {
  return tan(val);
}

float ATAN2(float val1,float val2) {
  return atan2(val2,val1);
}

float MOD(float val, int modulus) {
  return fmod(val, modulus);
}

float POWER(float val, int power) {
  return pow(val, power);
}

void p(float val){
  Serial.println(val);
}

//pass in seconds since 1/1/2000 by using DS3231 Library
// DateTime now = RTC.now(); //gets date
// seconds = now.get();
target getTargetAzimuth(uint32_t seconds) {
  target out;

  //offset from 2000 
  //- 1/2  to get reference from midnight
  //+ seconds/86400 convert seconds into days
  
  //E2 local time (in fraction of day)
  float LocalTime = (float)(seconds % 86400) / 86400.0;
  LocalTime -= .0000000003; //floating point error shift
  //p(LocalTime);
  //F2
  float JulianDay = (float)JulianDateNoonJan_1_2000 - 0.5 + ((float)seconds/86400);
  //p(JulianDay);
  //G2
  float JulianCentury = (-0.5+((float)seconds / 86400)) / 36525.0;
  JulianCentury -= .000000008; //floating point error shift
  //p(JulianCentury);
  //I2 -
  float GeomMeanLongSun = MOD(280.46646 + JulianCentury * (36000.76983 + JulianCentury * 0.0003032), 360);
  
  //J2
  float GeomMeanAnomSun = 357.52911 + JulianCentury * (35999.05029 - 0.0001537 * JulianCentury);
  
  //K2
  float EccentEarthOrbit = 0.016708634 - JulianCentury * (0.000042037 + 0.0000001267 * JulianCentury);
  
  //L2
  float SunEqOfCtr = SIN(RADIANS(GeomMeanAnomSun)) * (1.914602 - JulianCentury * (0.004817 + 0.000014 * JulianCentury)) + SIN(RADIANS(2 * GeomMeanAnomSun)) * (0.019993 - 0.000101 * JulianCentury) + SIN(RADIANS(3 * GeomMeanAnomSun)) * 0.000289;

  //M2- because of I2
  float SunTrueLong = GeomMeanLongSun + SunEqOfCtr;

  //N2
  float SunTrueAnom = GeomMeanAnomSun + SunEqOfCtr;
  
  //O2
  float SunRadVector = (1.000001018 * (1 - EccentEarthOrbit * EccentEarthOrbit)) / (1 + EccentEarthOrbit * COS(RADIANS(SunTrueAnom)));
  
  //P2 - because of I2
  float SunAppLong = SunTrueLong - 0.00569 - 0.00478 * SIN(RADIANS(125.04 - 1934.136 * JulianCentury));
  
  //Q2 
  float MeanObliqEcliptic = 23 + (26 + ((21.448 - JulianCentury * (46.815 + JulianCentury * (0.00059 - JulianCentury * 0.001813)))) / 60) / 60;
  
  //R2
  float ObliqCorr = MeanObliqEcliptic + 0.00256 * COS(RADIANS(125.04 - 1934.136 * JulianCentury));
  
  //S2 - because of I2
  float SunRtAscen = DEGREES(ATAN2(COS(RADIANS(SunAppLong)), COS(RADIANS(ObliqCorr)) * SIN(RADIANS(SunAppLong))));
  
  //T2 - because of I2
  float SunDeclination = DEGREES(ASIN(SIN(RADIANS(ObliqCorr)) * SIN(RADIANS(SunAppLong))));
  
  //U2
  float varY = TAN(RADIANS(ObliqCorr / 2)) * TAN(RADIANS(ObliqCorr / 2));
  
  //V2 - because of I2
  float EqOfTime = 4 * DEGREES(varY * SIN(2 * RADIANS(GeomMeanLongSun)) 
  - 2 * EccentEarthOrbit * SIN(RADIANS(GeomMeanAnomSun)) + 4 * EccentEarthOrbit * varY * SIN(RADIANS(GeomMeanAnomSun)) * COS(2 * RADIANS(GeomMeanLongSun))
  - 0.5 * varY * varY * SIN(4 * RADIANS(GeomMeanLongSun)) - 1.25 * EccentEarthOrbit * EccentEarthOrbit * SIN(2 * RADIANS(GeomMeanAnomSun)));
  
  //W2
  float HASunrise = DEGREES(ACOS(COS(RADIANS(90.833)) / (COS(RADIANS(latitude)) * COS(RADIANS(SunDeclination))) - TAN(RADIANS(latitude)) * TAN(RADIANS(SunDeclination))));
  
  //X2 - because of I2
  float SolarNoon = (720 - 4 * longitude - EqOfTime + timeZone * 60) / 1440;
  
  //Y2 - because of I2
  float SunriseTime = SolarNoon - HASunrise * 4 / 1440;
  
  //Z2 - because of I2
  float SunsetTime = SolarNoon + HASunrise * 4 / 1440;

  //AA2 (minutes)
  float SunlightDuration = 8 * HASunrise;
  
  //AB2 - because of I2
  float TrueSolarTime = MOD(LocalTime * 1440 + EqOfTime + 4 * longitude - 60 * timeZone, 1440);
  
  //AC2 radians
  float HourAngle = 0;
  if (TrueSolarTime / 4 < 0) {
    HourAngle = TrueSolarTime / 4 + 180;
  } else {
    HourAngle = TrueSolarTime / 4 - 180;
  }

  //AD2 radians
  float SolarZenithAngle = DEGREES(ACOS(SIN(RADIANS(latitude)) * SIN(RADIANS(SunDeclination)) + COS(RADIANS(latitude)) * COS(RADIANS(SunDeclination)) * COS(RADIANS(HourAngle))));
  
  //AE2
  float SolarElevationAngle = 90 - SolarZenithAngle;
  
  //AF2
  float AtmosphericRefraction;
  if (SolarElevationAngle > 85) {
  AtmosphericRefraction = 0; 
  }
  else if (SolarElevationAngle > 5) {
  AtmosphericRefraction = 58.1 / TAN(RADIANS(SolarElevationAngle)) - 0.07 / POWER(TAN(RADIANS(SolarElevationAngle)), 3)
    + 0.000086 / POWER(TAN(RADIANS(SolarElevationAngle)), 5);
  }
  else if (SolarElevationAngle > -0.575) {
    AtmosphericRefraction = 1735 + SolarElevationAngle * (-518.2 + SolarElevationAngle * (103.4 + SolarElevationAngle * (-12.79 + SolarElevationAngle * 0.711)));
  }
  else {
    AtmosphericRefraction = -20.772 / TAN(RADIANS(SolarElevationAngle));
  } 
  AtmosphericRefraction /= 3600;
  
  //AG2
  out.elevation = RADIANS(SolarElevationAngle + AtmosphericRefraction);

  //AH2
  if (HourAngle > 0) {
    out.azimuth = RADIANS(MOD(DEGREES(ACOS(((SIN(RADIANS(latitude)) * COS(RADIANS(SolarZenithAngle))) - SIN(RADIANS(SunDeclination))) / (COS(RADIANS(latitude)) * SIN(RADIANS(SolarZenithAngle))))) + 180, 360));
  } else {
    out.azimuth = RADIANS(MOD(540 - DEGREES(ACOS(((SIN(RADIANS(latitude)) * COS(RADIANS(SolarZenithAngle))) - SIN(RADIANS(SunDeclination))) / (COS(RADIANS(latitude)) * SIN(RADIANS(SolarZenithAngle))))), 360));
  }

  return out;
}
