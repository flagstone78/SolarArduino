target getTargetAzimuth(){
  target out;
  
  int n = 1; //JulianDate; //Julian day. 1<=n<=365
  float B = (360 * (n - 81)) / (364); //adjustment for time of year
  float E = 9.87*sin(2 * B) - 7.53*cos(B) - 1.5*sin(B); //solar time
  
  float Lst = 0; //standard meridian for the local time zones 
  // calculate from longitude
  
  float solarTime = 0;
  float standardTime = 0; //current time of day in hours (not daylight savings time)
  float hourAngle = 0; //angle of the sun east to west relative to south
  
  float declinationAngle = 0;
  float theta = 0; //angle towards hourAngle from normal to ground
  float azimuthAngle = 0;  //angle of the front of the platform relaive to south
  float azimuth = 0;

  n = 0;//getDayOfYear();
  standardTime = 0;//getTimeInHours();
    
  Lst = int((longitude+7.5)/15)*15; //takes of fraction
  solarTime = standardTime + 4*(Lst-longitude) + E;
  hourAngle = 0; //(solarTime*15)*(((2*PI)/360); //angle relative to south inradians to point at
  
  azimuthAngle = atan( sin(hourAngle)/(sin(latitude)*cos(hourAngle)-cos(latitude)*tan(declinationAngle)));
  declinationAngle = 23.45*sin((360/365)*(284+n));
  theta = acos(
      sin(declinationAngle)*sin(latitude)*cos(slope) 
    - sin(declinationAngle)*cos(latitude)*sin(slope)*cos(azimuth)
    + cos(declinationAngle)*cos(latitude)*cos(slope)*cos(hourAngle)
    + cos(declinationAngle)*sin(latitude)*sin(slope)*cos(azimuth)*cos(hourAngle)
    + cos(declinationAngle)*sin(slope)*sin(azimuth)*sin(hourAngle)
    );  

  out.azimuth = -azimuthAngle;
  out.zenith = 90-theta;

  return out;

}

