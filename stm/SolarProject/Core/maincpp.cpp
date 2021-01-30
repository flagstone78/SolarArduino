/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */


#include "maincpp.h"

TIM_HandleTypeDef htim1;
RTC_HandleTypeDef hrtc;

//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, true, -5);

//motors
Stepper elStepper(ElStepperPorts,ElStepperPins, true);

//clock
//RTC_DS3231 rtc;

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
RTC_DateTypeDef dateS;
RTC_TimeTypeDef timeS;
time_t timeSeconds;
tm tms;

AzEl Target;

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

	//date = rtc.now();
	HAL_RTC_GetTime(&hrtc,&timeS,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&dateS,RTC_FORMAT_BIN);
	//timeSeconds = date.secondstime();
	const int century = 1; //centuries since 1900
	tms = {timeS.Seconds,timeS.Minutes,timeS.Hours,dateS.Date,dateS.Month-1,dateS.Year+century*100,0,0,(int)timeS.DayLightSaving};

	timeSeconds = mktime(&tms); //update mday and yday
	Target = calculateSolar(tms,geo);

	targetAngle = Target.Elevation;
	if(targetAngle < 5) targetAngle = 5;
	if(targetAngle > 90) targetAngle = 90;
}
