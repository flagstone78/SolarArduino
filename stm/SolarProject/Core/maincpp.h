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
#include "../RTCLib.h"
extern "C" {
	void mainsetup();
	void mainloop();
}


#endif /* MAINCPP_H_ */