/*
 * absEncoder.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#include "absEncoder.h"

absEncoder::absEncoder(const GPIO_TypeDef* const gpioPorts[10], const uint16_t gpioPins[10], float operatingPoint, bool reverseDirection, float offset)
:ports(gpioPorts),pins(gpioPins),opPoint(operatingPoint),reverse(reverseDirection),degreeOffset(offset) {

}

absEncoder::~absEncoder() {
	// TODO Auto-generated destructor stub
}

uint16_t absEncoder::positionRaw(){
	// read all gpio at once
	uint16_t a = GPIOA->IDR;
	uint16_t b = GPIOB->IDR;
	uint16_t c = GPIOC->IDR;

	//construct greycode
	uint16_t grey = 0;
	uint16_t p = 0;
	uint8_t bit = 0;
	for(int i = 0; i<10; i++){
		if(ports[i]==GPIOA){
			p = a;
		}else if(ports[i]==GPIOB){
			p = b;
		}else if(ports[i]==GPIOC){
			p = c;
		}
		bit = (p&pins[i])>0;
		grey += bit<<i;
	}
	grey = (0xffff^grey)&0x03ff; //bitwise invert because the encoder is active low
	return grey;
}

//between 0 and 1024
uint16_t absEncoder::positionInt(){
	return grayToBinary(positionRaw());
}

//return the position of the encoder in degrees opPoint-180.0 to opPoint+180.0 range
float absEncoder::position(){
	float deg = positionInt()*360.0/1024.0;
	if(reverse) deg = -deg;
	deg += degreeOffset;
	if(deg != 0) deg = fmod(deg,360.0); //between -360 to 360
	if(deg < opPoint-180.0)deg+=360;
	else if(deg > opPoint+180.0)deg-=360;
	//cachedPosition = deg;
	return deg;
}

uint16_t grayToBinary(uint16_t num){
	uint16_t mask = num;
	while (mask) {           // Each Gray code bit is exclusive-ored with all more significant bits.
		mask >>= 1;
		num   ^= mask;
	}
	return num;
}
