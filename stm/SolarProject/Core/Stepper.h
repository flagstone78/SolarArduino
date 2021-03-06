/*
 * Stepper.h
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#ifndef SRC_STEPPER_H_
#define SRC_STEPPER_H_

#include "GLOBALS.h"

class Stepper {
	const GPIO_TypeDef*const* ports;
	const uint16_t* pins;
	//int currentStep, targetStep;
	const bool reverse;
public:
	Stepper(const GPIO_TypeDef* const gpioPorts[3], const uint16_t gpioPins[3], bool reverseDirection);
	virtual ~Stepper();
	void step();
	void Enable(bool ena);
	void setDir(bool dir);
	//void update();
	//void setFreq(float Hz);
	//void setTarget(int pos);
	//unsigned int getPosition();
};

#endif /* SRC_STEPPER_H_ */
