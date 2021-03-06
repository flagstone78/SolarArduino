/*
 * maincpp.h
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#ifndef MAINCPP_H_
#define MAINCPP_H_

#include "main.h"
#include "../absEncoder.h"
#include "../Stepper.h"
#include "../MotorControl.h"
//#include "../RTCLib.h"
//#include "../SolarCalculations.h"
#include "../solarCalc2.h"
#include <time.h>

const Geo geo = {
		 -6, //time zone of the clock (preferred gmt)
	44.0121, //Latitude of the solar collector, needs to be changed for current location, rochester Mn
	-92.4802 //Longitude of the solar collector, needs to be changed for current location, current Rochester Mn
};

extern "C" {
	void mainsetup();
	void mainloop();
}


#endif /* MAINCPP_H_ */
