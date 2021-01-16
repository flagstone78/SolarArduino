/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */


#include "maincpp.h"

TIM_HandleTypeDef htim1;

//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, false, 88.6875);

//motors
Stepper elStepper(ElStepperPorts,ElStepperPins, false);

// callback function for stepper motor timer
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim1)
  {
	  elStepper.update();
  }
}

int currentpos;
float pos, oldpos, targetAngle;
void mainsetup(){
	elStepper.setFreq(100);
	currentpos = 0;
	oldpos = 0;
	pos=0;
	targetAngle = elEncoder.position();
}

void mainloop(){
	pos = elEncoder.position(); //get encoder angle position
	float err = targetAngle-pos; //feedback
	float vel = 100*err; //proportional term

	//set velocity
	elStepper.setDir(vel > 0);
	elStepper.setFreq(abs(vel));
}
