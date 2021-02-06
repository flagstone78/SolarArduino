/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */


#include "maincpp.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
RTC_HandleTypeDef hrtc;

//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, 0, true, -5);
absEncoder azEncoder(AzEncoderPorts,AzEncoderPins, 180, false, 183);

//motors
Stepper elStepper(&htim1,ElStepperPorts,ElStepperPins, true);
Stepper azStepper(&htim2,AzStepperPorts,AzStepperPins, true);

//clock
//RTC_DS3231 rtc;

// callback function for stepper motor timer
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == elStepper.timer) {
		elStepper.update();
	}
	if(htim == azStepper.timer) {
		azStepper.update();
	}
}


int currentpos;
float elpos, azpoz, oldpos, targetAngle=0;
RTC_DateTypeDef dateS;
RTC_TimeTypeDef timeS;
time_t timeSeconds;
tm tms;

AzEl Target;

void mainsetup(){
	elStepper.setFreq(100);
	azStepper.setFreq(100);
}

void motorControl(Stepper* s, absEncoder* e, float targetAngle){
	float pos = e->position();
	float err = targetAngle-pos; //feedback

	float p = 200;
	if (abs(err) < 1.0) p = 20;
	float vel = p*err; //proportional term

	//set velocity
	s->setDir(vel > 0);
	s->setFreq(abs(vel));
}

void limit(double *var, double high, double low){
	if(*var < low) *var = low;
	else if(*var > high) *var = high;
}

void mainloop(){
	//get the time (must be called getTime then getDate to work properly)
	HAL_RTC_GetTime(&hrtc,&timeS,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&dateS,RTC_FORMAT_BIN);

	const int century = 1; //centuries since 1900
	tms = {timeS.Seconds,timeS.Minutes,timeS.Hours,dateS.Date,dateS.Month-1,dateS.Year+century*100,0,0,(int)timeS.DayLightSaving};

	timeSeconds = mktime(&tms); //update mday and yday
	Target = calculateSolar(tms,geo);

	limit(&Target.Elevation, 90,5);
	limit(&Target.Azimuth, 300,60);

	motorControl(&elStepper, &elEncoder, Target.Elevation);
	motorControl(&azStepper, &azEncoder, Target.Azimuth);
}
