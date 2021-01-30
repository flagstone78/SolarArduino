/*
 * solar2Calc2.h
 *
 *  Created on: Jan 30, 2021
 *      Author: Duane Mathias
 */

#ifndef SOLARCALC2_H_
#define SOLARCALC2_H_

#include <time.h>
#include <math.h>
#include "main.h"

struct ymd{
	int year;
	int month;
	double day;
};

struct AzEl{
	double Azimuth;
	double Elevation;
};

struct Geo{
	double tz; //time zone
	double lat; //latitude
	double lon; //longitude
};

struct SunRiseSet{
	double jday; //julian day
	double timeLocal;//time in minutes from midnight
	double azimuth; //sun set/sunrise location
};

AzEl calculateSolar(tm tms, Geo geo);

double radToDeg(double angleRad);

double degToRad(double angleDeg);


#endif /* SOLARCALC2_H_ */
