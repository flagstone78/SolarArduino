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
	const float opPoint;  // return angle in range of opPoint-180 to opPoint+180; opPoint must be between -180 to 180
	const bool reverse; // count + as clockwise or counter clock wise
	const float degreeOffset; //difference between encoder and machine angle
	//float cachedPosition;
public:

	absEncoder(const GPIO_TypeDef* const gpioPorts[10], const uint16_t gpioPins[10], float operatingPoint, bool reverseDirection, float offset);
	virtual ~absEncoder();
	uint16_t positionRaw(); //greycode value
	uint16_t positionInt(); //0 to 1024
	float position(); //in degrees opPoint-180.0 to opPoint+180.0 range
	//float getCachedPosition(){return cachedPosition;}
};

uint16_t grayToBinary(uint16_t num);

#endif /* ABSENCODER_H_ */
