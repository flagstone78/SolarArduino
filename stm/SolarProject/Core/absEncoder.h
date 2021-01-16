/*
 * absEncoder.h
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#ifndef ABSENCODER_H_
#define ABSENCODER_H_

//#include "stm32f4xx_hal.h"
#include "GLOBALS.h"
#include <math.h>

class absEncoder {
	const GPIO_TypeDef*const* ports;
	const uint16_t* pins;
	const bool reverse;
	const float degreeOffset;
public:
	absEncoder(const GPIO_TypeDef* const gpioPorts[10], const uint16_t gpioPins[10], bool reverseDirection, float offset);
	virtual ~absEncoder();
	uint16_t positionRaw();
	float position();
};

uint16_t grayToBinary(uint16_t num);

#endif /* ABSENCODER_H_ */
