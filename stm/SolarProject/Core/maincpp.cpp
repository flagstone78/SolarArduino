/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */


#include "maincpp.h"

TIM_HandleTypeDef htim1;

//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, -1.5);

//motors
Stepper elStepper(ElStepperPorts,ElStepperPins);

// callback function for stepper motor timer
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  // Check which version of the timer triggered this callback and toggle LED
  if (htim == &htim1)
  {
    //HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	  elStepper.update();
  }
}

int target, currentpos;
float pos, oldpos;
void mainsetup(){
	elStepper.setFreq(100);
	target=350;
	currentpos = 0;
	oldpos = 0;
	pos=0;
}

void mainloop(){
	pos = elEncoder.position();
	currentpos = elStepper.getPosition();
	if(pos<target)elStepper.setTarget(currentpos-1);
	else elStepper.setTarget(currentpos+1);
}
