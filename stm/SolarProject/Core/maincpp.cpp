/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */


#include "maincpp.h"

TIM_HandleTypeDef htim1;

//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, true, -5);

//motors
Stepper elStepper(ElStepperPorts,ElStepperPins, true);

//clock
RTC_DS3231 rtc;

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
DateTime date;
long timeSeconds;
target Target;

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

	float p = 200;
	if (abs(err) < 1.0) p = 20;
	float vel = p*err; //proportional term

	//set velocity
	elStepper.setDir(vel > 0);
	elStepper.setFreq(abs(vel));

	date = rtc.now();
	timeSeconds = date.secondstime();
	Target = getTargetAzimuth(timeSeconds);

	targetAngle = DEGREES(Target.elevation);
	if(targetAngle < 5) targetAngle = 5;
	if(targetAngle > 90) targetAngle = 90;
}
