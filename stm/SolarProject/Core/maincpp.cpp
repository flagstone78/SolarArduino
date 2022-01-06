/*
 * maincpp.cpp
 *
 *  Created on: Jan 2, 2021
 *      Author: Alan Mathias
 */


#include "maincpp.h"


TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2; //to debugger

//encoders
absEncoder elEncoder(ElEncoderPorts,ElEncoderPins, 0, true, 0.4);
absEncoder azEncoder(AzEncoderPorts,AzEncoderPins, 180, false, 186);//173
//absEncoder azEncoder(AzEncoderPorts,AzEncoderPins, 0, false, 0);

//motors
Stepper elStepper(ElStepperPorts,ElStepperPins, false);
Stepper azStepper(AzStepperPorts,AzStepperPins, false);

MotorControl azControl(&htim1,&azEncoder,&azStepper);
MotorControl elControl(&htim2,&elEncoder,&elStepper);

//gps
TinyGPSPlus gps;

// callback function for stepper motor timer
bool enableMotors = true;
bool manualControl = false;
bool updateTime = true;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == elControl.timer) {
		elControl.update(enableMotors);
	}
	if(htim == azControl.timer) {
		azControl.update(enableMotors);
	}
}

void customUART(char c){
	gps.encode(c);
}


int azRaw;
bool dir = false;
float elpos, azpos, oldpos, targetAngle=0;
time_t timeSeconds;
tm tms;
uint32_t oldMillis=0;

char printBuf[300];

AzEl Target;

void mainsetup(){
	elStepper.Enable(true);
	azStepper.Enable(true);

	Target.Elevation = elEncoder.position();
	Target.Azimuth = azEncoder.position();
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
	if((millis()-oldMillis) > 1000){
		oldMillis = millis();
	sprintf(printBuf,"lati: %f\n\r long: %f\n\r date: %ld\n\r time: %ld\n\r Az: %f \n\r AzT: %f\n\r El: %f\n\r ElT: %f\n\r",
			gps.location.lat(), gps.location.lng(), gps.date.value(), gps.time.value(),
			azEncoder.position(), Target.Azimuth,
			elEncoder.position(), Target.Elevation);
	HAL_UART_Transmit(&huart2, (uint8_t *)printBuf, sizeof(printBuf), 50);
	}

	//get the time from gps module  (gps is update from the customUART function from uart1 interrupt)
	if(updateTime && gps.time.isValid()){
		tms.tm_year = gps.date.year()%100 + century*100; //years from 1900
		tms.tm_mon = gps.date.month()-1; //month is zero indexed
		tms.tm_mday = gps.date.day();
		tms.tm_hour = gps.time.hour();
		tms.tm_min = gps.time.minute();
		tms.tm_sec = gps.time.second();
		timeSeconds = mktime(&tms); //update mday and yday
	}

	if(gps.location.isValid()){
		geo.lat = gps.location.lat();
		geo.lon = gps.location.lng();
	}

	if(!manualControl && gps.time.isValid()){
		Target = calculateSolar(tms,geo);
	}
	limit(&Target.Elevation, 90,5);
	limit(&Target.Azimuth, 300,60);
	elControl.setTargetAngle(Target.Elevation);
	azControl.setTargetAngle(Target.Azimuth);
}
