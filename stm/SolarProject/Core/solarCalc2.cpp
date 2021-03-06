#include "solarCalc2.h"

/*************************************************************/
/* Solar position calculation functions */
/*************************************************************/

double calcTimeJulianCent(double jd) {
	double T = (jd - 2451545.0)/36525.0;
	return T;
}

double calcJDFromJulianCent(double t) {
	double JD = t * 36525.0 + 2451545.0;
	return JD;
}

bool isLeapYear(int yr) {
	return ((yr % 4 == 0 && yr % 100 != 0) || yr % 400 == 0);
}

ymd calcDateFromJD(double jd) {
	double z = floor(jd + 0.5);
	double f = (jd + 0.5) - z;
	double A, alpha;
	if (z < 2299161) {
		A = z;
	} else {
		alpha = floor((z - 1867216.25)/36524.25);
		A = z + 1 + alpha - floor(alpha/4);
	}
	int B = A + 1524;
	int C = floor((B - 122.1)/365.25);
	int D = floor(365.25 * C);
	int E = floor((B - D)/30.6001);
	double day = B - D - floor(30.6001 * E) + f;
	int month = (E < 14) ? E - 1 : E - 13;
	int year = (month > 2) ? C - 4716 : C - 4715;

	return ymd{ year,  month,  day};
}

double calcDoyFromJD(double jd) {
	ymd date = calcDateFromJD(jd);

	int k = (isLeapYear(date.year) ? 1 : 2);
	double doy = floor((275 * date.month)/9) - k * floor((date.month + 9)/12) + date.day -30;

	return doy;
}


double radToDeg(double angleRad) {
	return (180.0 * angleRad / M_PI);
}

double degToRad(double angleDeg) {
	return (M_PI * angleDeg / 180.0);
}

double calcGeomMeanLongSun(double t) {
	double L0 = 280.46646 + t * (36000.76983 + t*(0.0003032));
	while(L0 > 360.0) {
		L0 -= 360.0;
	}
	while(L0 < 0.0) {
		L0 += 360.0;
	}
	return L0;		// in degrees
}

double calcGeomMeanAnomalySun(double t) {
	double M = 357.52911 + t * (35999.05029 - 0.0001537 * t);
	return M;		// in degrees
}

double calcEccentricityEarthOrbit(double t) {
	double e = 0.016708634 - t * (0.000042037 + 0.0000001267 * t);
	return e;		// unitless
}

double calcSunEqOfCenter(double t) {
	double m = calcGeomMeanAnomalySun(t);
	double mrad = degToRad(m);
	double sinm = sin(mrad);
	double sin2m = sin(mrad+mrad);
	double sin3m = sin(mrad+mrad+mrad);
	double C = sinm * (1.914602 - t * (0.004817 + 0.000014 * t)) + sin2m * (0.019993 - 0.000101 * t) + sin3m * 0.000289;
	return C;		// in degrees
}

double calcSunTrueLong(double t) {
	double l0 = calcGeomMeanLongSun(t);
	double c = calcSunEqOfCenter(t);
	double O = l0 + c;
	return O;		// in degrees
}

double calcSunTrueAnomaly(double t) {
	double m = calcGeomMeanAnomalySun(t);
	double c = calcSunEqOfCenter(t);
	double v = m + c;
	return v;		// in degrees
}

double calcSunRadVector(double t) {
	double v = calcSunTrueAnomaly(t);
	double e = calcEccentricityEarthOrbit(t);
	double R = (1.000001018 * (1 - e * e)) / (1 + e * cos(degToRad(v)));
	return R;		// in AUs
}

double calcSunApparentLong(double t) {
	double o = calcSunTrueLong(t);
	double omega = 125.04 - 1934.136 * t;
	double lambda = o - 0.00569 - 0.00478 * sin(degToRad(omega));
	return lambda;		// in degrees
}

double calcMeanObliquityOfEcliptic(double t) {
	double seconds = 21.448 - t*(46.8150 + t*(0.00059 - t*(0.001813)));
	double e0 = 23.0 + (26.0 + (seconds/60.0))/60.0;
	return e0;		// in degrees
}

double calcObliquityCorrection(double t) {
	double e0 = calcMeanObliquityOfEcliptic(t);
	double omega = 125.04 - 1934.136 * t;
	double e = e0 + 0.00256 * cos(degToRad(omega));
	return e;		// in degrees
}

double calcSunRtAscension(double t) {
	double e = calcObliquityCorrection(t);
	double lambda = calcSunApparentLong(t);
	double tananum = (cos(degToRad(e)) * sin(degToRad(lambda)));
	double tanadenom = (cos(degToRad(lambda)));
	double alpha = radToDeg(atan2(tananum, tanadenom));
	return alpha;		// in degrees
}

double calcSunDeclination(double t) {
	double e = calcObliquityCorrection(t);
	double lambda = calcSunApparentLong(t);
	double sint = sin(degToRad(e)) * sin(degToRad(lambda));
	double theta = radToDeg(asin(sint));
	return theta;		// in degrees
}

double calcEquationOfTime(double t) {
	double epsilon = calcObliquityCorrection(t);
	double l0 = calcGeomMeanLongSun(t);
	double e = calcEccentricityEarthOrbit(t);
	double m = calcGeomMeanAnomalySun(t);

	double y = tan(degToRad(epsilon)/2.0);
	y *= y;

	double sin2l0 = sin(2.0 * degToRad(l0));
	double sinm   = sin(degToRad(m));
	double cos2l0 = cos(2.0 * degToRad(l0));
	double sin4l0 = sin(4.0 * degToRad(l0));
	double sin2m  = sin(2.0 * degToRad(m));

	double Etime = y * sin2l0 - 2.0 * e * sinm + 4.0 * e * y * sinm * cos2l0 - 0.5 * y * y * sin4l0 - 1.25 * e * e * sin2m;
	return radToDeg(Etime)*4.0;	// in minutes of time
}

double calcHourAngleSunrise(double lat, double solarDec) {
	double latRad = degToRad(lat);
	double sdRad  = degToRad(solarDec);
	double HAarg = (cos(degToRad(90.833))/(cos(latRad)*cos(sdRad))-tan(latRad) * tan(sdRad));
	if(HAarg > 1 || HAarg < -1) return -1;
	else return acos(HAarg); // in radians (for sunset, use -HA)
}

double getJD(int year, int month, double day) {
	if (month <= 2) {
		year -= 1;
		month += 12;
	}
	int A = floor(year/100);
	int B = 2 - A + floor(A/4);
	double JD = floor(365.25*(year + 4716)) + floor(30.6001*(month+1)) + day + B - 1524.5;
	return JD;
}

double calcRefraction(double elev) {
	double correction;
	if (elev > 85.0) {
		correction = 0.0;
	} else {
		double te = tan(degToRad(elev));
		if (elev > 5.0) {
			correction = 58.1 / te - 0.07 / (te*te*te) + 0.000086 / (te*te*te*te*te);
		} else if (elev > -0.575) {
			correction = 1735.0 + elev * (-518.2 + elev * (103.4 + elev * (-12.79 + elev * 0.711) ) );
		} else {
			correction = -20.774 / te;
		}
		correction = correction / 3600.0;
	}

	return correction;
}

AzEl calcAzEl(double T, double localtime, double latitude, double longitude, double zone) {

	double eqTime = calcEquationOfTime(T);
	double theta  = calcSunDeclination(T);

	double solarTimeFix = eqTime + 4.0 * longitude - 60.0 * zone;
	//double earthRadVec = calcSunRadVector(T);
	double trueSolarTime = localtime + solarTimeFix;
	while (trueSolarTime > 1440) {
		trueSolarTime -= 1440;
	}
	double hourAngle = trueSolarTime / 4.0 - 180.0;
	if (hourAngle < -180) {
		hourAngle += 360.0;
	}
	double haRad = degToRad(hourAngle);
	double csz = sin(degToRad(latitude)) * sin(degToRad(theta)) + cos(degToRad(latitude)) * cos(degToRad(theta)) * cos(haRad);
	if (csz > 1.0) {
		csz = 1.0;
	} else if (csz < -1.0) {
		csz = -1.0;
	}
	double zenith = radToDeg(acos(csz));
	double azDenom = ( cos(degToRad(latitude)) * sin(degToRad(zenith)) );
	double azimuth;
	if ( abs(azDenom) > 0.001) {
		double azRad = (( sin(degToRad(latitude)) * cos(degToRad(zenith)) ) - sin(degToRad(theta))) / azDenom;
		if (abs(azRad) > 1.0) {
			if (azRad < 0) {
				azRad = -1.0;
			} else {
				azRad = 1.0;
			}
		}
		azimuth = 180.0 - radToDeg(acos(azRad));
		if (hourAngle > 0.0) {
			azimuth = -azimuth;
		}
	} else {
		if (latitude > 0.0) {
			azimuth = 180.0;
		} else {
			azimuth = 0.0;
		}
	}
	if (azimuth < 0.0) {
		azimuth += 360.0;
	}
	double exoatmElevation = 90.0 - zenith;

	// Atmospheric Refraction correction
	double refractionCorrection = calcRefraction(exoatmElevation);

	double solarZen = zenith - refractionCorrection;
	double elevation = 90.0 - solarZen;

	return AzEl{azimuth, elevation};
}

double calcSolNoon(double jd, double longitude, double timezone) {
	double tnoon = calcTimeJulianCent(jd - longitude/360.0);
	double eqTime = calcEquationOfTime(tnoon);
	double solNoonOffset = 720.0 - (longitude * 4) - eqTime; // in minutes
	double newt = calcTimeJulianCent(jd + solNoonOffset/1440.0);
	eqTime = calcEquationOfTime(newt);
	double solNoonLocal = 720 - (longitude * 4) - eqTime + (timezone*60.0);// in minutes
	while (solNoonLocal < 0.0) {
		solNoonLocal += 1440.0;
	}
	while (solNoonLocal >= 1440.0) {
		solNoonLocal -= 1440.0;
	}

	return solNoonLocal;
}


double calcSunriseSetUTC(bool rise, double JD, double latitude, double longitude) {
	double t = calcTimeJulianCent(JD);
	double eqTime = calcEquationOfTime(t);
	double solarDec = calcSunDeclination(t);
	double hourAngle = calcHourAngleSunrise(latitude, solarDec);
	if(hourAngle<0) return -1;  //TODO  this error handling

	if (!rise) hourAngle = -hourAngle;
	double delta = longitude + radToDeg(hourAngle);
	double timeUTC = 720 - (4.0 * delta) - eqTime;	// in minutes

	return timeUTC;
}

double calcJDofNextPrevRiseSet(bool next, bool rise, double JD, double latitude, double longitude, double tz) {

	double julianday = JD;
	double increment = ((next) ? 1.0 : -1.0);
	double time = calcSunriseSetUTC(rise, julianday, latitude, longitude);

	while(time<0) {
		julianday += increment;
		time = calcSunriseSetUTC(rise, julianday, latitude, longitude);
	}
	double timeLocal = time + tz * 60.0;
	while ((timeLocal < 0.0) || (timeLocal >= 1440.0)) {
		double incr = ((timeLocal < 0) ? 1 : -1);
		timeLocal += (incr * 1440.0);
		julianday -= incr;
	}

	return julianday;
}

 // rise = 1 for sunrise, 0 for sunset
SunRiseSet calcSunriseSet(bool rise, double JD, double latitude, double longitude, double timezone) {
	double timeUTC, newTimeUTC,azimuth,timeLocal,jday;

	timeUTC = calcSunriseSetUTC(rise, JD, latitude, longitude);
	newTimeUTC = calcSunriseSetUTC(rise, JD + timeUTC/1440.0, latitude, longitude);

	if (newTimeUTC >= 0 ) {
		timeLocal = newTimeUTC + (timezone * 60.0);
		double riseT = calcTimeJulianCent(JD + newTimeUTC/1440.0);
		AzEl riseAzEl = calcAzEl(riseT, timeLocal, latitude, longitude, timezone);
		azimuth = riseAzEl.Azimuth;
		jday = JD;
		if ( (timeLocal < 0.0) || (timeLocal >= 1440.0) ) {
			double increment = ((timeLocal < 0) ? 1 : -1);
			while ((timeLocal < 0.0)||(timeLocal >= 1440.0)) {
				timeLocal += increment * 1440.0;
				jday -= increment;
			}
		}
	}else{ // no sunrise/set found
		azimuth = -1.0;
		timeLocal = 0.0;
		double doy = calcDoyFromJD(JD);
		if ( ((latitude > 66.4) && (doy > 79) && (doy < 267)) ||
			 ((latitude < -66.4) && ((doy < 83) || (doy > 263))) ) {
			//previous sunrise/next sunset
			jday = calcJDofNextPrevRiseSet(!rise, rise, JD, latitude, longitude, timezone);
		} else {   //previous sunset/next sunrise
			jday = calcJDofNextPrevRiseSet(rise, rise, JD, latitude, longitude, timezone);
		}
	}

	return SunRiseSet{ jday,  timeLocal, azimuth};
}


/*************************************************************/
/* end calculation functions */
/*************************************************************/


//--------------------------------------------------------------
// Do the calculations and update the result text boxes


AzEl calculateSolar(tm tms, Geo geo) {

	double time_local =  tms.tm_hour*60 + tms.tm_min + tms.tm_sec/60.0; //local time in minutes

	double jday = getJD(tms.tm_year+1900, tms.tm_mon+1, tms.tm_mday);
	double total = jday + time_local/1440.0 - geo.tz/24.0;
	double T = calcTimeJulianCent(total);
	return calcAzEl(T, time_local, geo.lat, geo.lon, geo.tz);
	//double solnoon = calcSolNoon(jday, geo.lon, geo.tz);
	//SunRiseSet rise = calcSunriseSet(1, jday, geo.lat, geo.lon, geo.tz);
	//SunRiseSet set  = calcSunriseSet(0, jday, geo.lat, geo.lon, geo.tz);

	//double eqTime = calcEquationOfTime(T);
	//double theta  = calcSunDeclination(T);
}
