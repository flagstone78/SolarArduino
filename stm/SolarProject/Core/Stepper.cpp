/*
 * Stepper.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */

#include "Stepper.h"

Stepper::Stepper(const GPIO_TypeDef* const gpioPorts[3], const uint16_t gpioPins[3], const bool reverseDirection)
:ports(gpioPorts),pins(gpioPins), reverse(reverseDirection) {

}

Stepper::~Stepper() {
	// TODO Auto-generated destructor stub
}

void Stepper::Enable(bool ena){
	HAL_GPIO_WritePin((GPIO_TypeDef*)ports[2],pins[2],(GPIO_PinState)!ena);
}

void Stepper::setDir(bool dir){
	if(reverse) dir = !dir;
	HAL_GPIO_WritePin((GPIO_TypeDef*)ports[1],pins[1],(GPIO_PinState)dir);
}

//always take a step
void Stepper::step() {
	HAL_GPIO_TogglePin((GPIO_TypeDef*) ports[0], pins[0]);
}

/////called from timers
//void Stepper::update(){
//	if(!stopped) step();
//}

////stops if freq is less than 1
//void Stepper::setFreq(float Hz){
//	if(Hz < 1) {Hz = 1; stopped = true;}else{stopped = false;}
//	if(Hz > 2000) Hz = 2000;
//	timer->Instance->ARR = (uint16_t)(65535.0/Hz);
//}

//void Stepper::setTarget(int pos){
//	targetStep=pos;
//}

//unsigned int Stepper::getPosition(){
//	return currentStep;
//}
