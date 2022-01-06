/*
 * maincpp.h
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#ifndef MAINCPP_H_
#define MAINCPP_H_

#include "main.h"
#include <stdio.h>
#include <string.h>
#include "../absEncoder.h"
#include "../Stepper.h"
#include "../MotorControl.h"
//#include "../RTCLib.h"
//#include "../SolarCalculations.h"
#include "../solarCalc2.h"
#include <time.h>
#include "../TinyGPS++.h"


const unsigned int century = 1; //centuries since 1900 (needed to construct time from GPS)
Geo geo = {
	0, //time zone of the clock (preferred gmt/utc)
	0, //Latitude of the solar collector, needs to be changed for current location, rochester Mn
	0 //Longitude of the solar collector, needs to be changed for current location, current Rochester Mn
};

extern "C" {
	void mainsetup();
	void mainloop();
	void customUART(char c);
}


#endif /* MAINCPP_H_ */
