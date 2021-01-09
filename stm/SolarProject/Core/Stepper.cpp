/*
 * Stepper.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#include "Stepper.h"

Stepper::Stepper(const GPIO_TypeDef* const gpioPorts[3], const uint16_t gpioPins[3]):ports(gpioPorts),pins(gpioPins) {
	currentStep = 0;
	targetStep = 0;
}

Stepper::~Stepper() {
	// TODO Auto-generated destructor stub
}

void Stepper::setDir(GPIO_PinState dir){
	HAL_GPIO_WritePin((GPIO_TypeDef*)ports[1],pins[1],dir);
}

void Stepper::step() {
	bool dirState = ((ports[1]->ODR & pins[1]) == pins[1]);
	if (dirState){
		currentStep += 1;
		HAL_GPIO_TogglePin((GPIO_TypeDef*) ports[0], pins[0]);
	} else if(!dirState){
		currentStep -= 1;
		HAL_GPIO_TogglePin((GPIO_TypeDef*)ports[0], pins[0]);
	}
}

void Stepper::update(){
	setDir((GPIO_PinState)(currentStep < targetStep));
	if(currentStep!=targetStep) step();
}

void Stepper::setFreq(unsigned int Hz){
	if(Hz < 5000){
		TIM1->ARR = 65535/Hz;
	}
}

void Stepper::setTarget(int pos){
	targetStep=pos;
}

unsigned int Stepper::getPosition(){
	return currentStep;
}
