/*
 * absEncoder.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#include "absEncoder.h"

absEncoder::absEncoder(const GPIO_TypeDef* const gpioPorts[10], const uint16_t gpioPins[10]):ports(gpioPorts),pins(gpioPins) {

}

absEncoder::~absEncoder() {
	// TODO Auto-generated destructor stub
}

uint16_t absEncoder::position(){
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

	//convert to binary
	uint16_t bin = grey & (1<<9); //copy MSB
	for(int i=8;i>=0;i--){
		bin += ((bin & (1<<(i+1)))>0)^((grey&(1<<i))>0);
	}
	return bin;
}
