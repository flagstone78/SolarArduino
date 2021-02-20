/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Duane Mathias
 */


#include "maincpp.h"
#include <stdio.h>

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
RTC_HandleTypeDef hrtc;
UART_HandleTypeDef huart2;



//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, 0, true, 0.4);
absEncoder azEncoder(AzEncoderPorts,AzEncoderPins, 180, false, 173);
//absEncoder azEncoder(AzEncoderPorts,AzEncoderPins, 0, false, 0);

//motors
Stepper elStepper(&htim1,ElStepperPorts,ElStepperPins, false);
Stepper azStepper(&htim2,AzStepperPorts,AzStepperPins, true);

//clock
//RTC_DS3231 rtc;

// callback function for stepper motor timer
bool enableMotors = false;
bool manualControl = false;
bool updateTime = true;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(enableMotors){
	if(htim == elStepper.timer) {
		elStepper.update();
	}
	if(htim == azStepper.timer) {
		azStepper.update();
	}
	}
}


int azRaw;
bool dir = false;
float elpos, azpos, oldpos, targetAngle=0;
RTC_DateTypeDef dateS;
RTC_TimeTypeDef timeS;
time_t timeSeconds;
tm tms;

char printBuf[50];

AzEl Target;

void mainsetup(){
	elStepper.setFreq(100);
	azStepper.setFreq(100);

	elStepper.Enable(true);
	azStepper.Enable(true);

	Target.Elevation = elEncoder.position();
	Target.Azimuth = azEncoder.position();
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

int bin2int(int b){
	int o = 0;
	for(int i=0;i<10;i++){
		if(b&(1<<i)){
			o += pow(10,i);
		}
	}
	return o;
}

void mainloop(){

	/*azpos = azEncoder.position();
	azRaw = bin2int(azEncoder.positionRaw());
	sprintf(printBuf,"a: %f b: %i\n",azpos,azRaw);
	HAL_UART_Transmit(&huart2, (uint8_t *)printBuf, sizeof(printBuf), 50);

	azStepper.setDir(dir); //false is to negative
	if(azpos>60 && azpos < 300){
		azStepper.step();
		for(int i=10000;i>0;i--){}
	}*/


	//get the time (must be called getTime then getDate to work properly)
	HAL_RTC_GetTime(&hrtc,&timeS,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&dateS,RTC_FORMAT_BIN);

	if(updateTime){
		const int century = 1; //centuries since 1900
		tms = {timeS.Seconds,timeS.Minutes,timeS.Hours,dateS.Date,dateS.Month-1,dateS.Year+century*100,0,0,(int)timeS.DayLightSaving};
		timeSeconds = mktime(&tms); //update mday and yday
	}

	if(!manualControl){
		Target = calculateSolar(tms,geo);
	}

	limit(&Target.Elevation, 90,5);
	limit(&Target.Azimuth, 300,60);

	motorControl(&elStepper, &elEncoder, Target.Elevation);
	motorControl(&azStepper, &azEncoder, Target.Azimuth);

}
