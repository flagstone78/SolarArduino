/*
 * MotorControl.h
 *
 *  Created on: Mar 6, 2021
 *      Author: Duane Mathias
 */

#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_


class MotorControl{
	float targetAngle, encoderPos, error;
	float pgain = 200;
	absEncoder* const encoder;
	Stepper* const stepper;

public:
	TIM_HandleTypeDef* const timer;



	MotorControl(TIM_HandleTypeDef* timer, absEncoder* encoder, Stepper* stepper):encoder(encoder),stepper(stepper),timer(timer){
		setTimer(10);
	};
	void setTimer(float stepsPerSecond){
		if(stepsPerSecond < 1) stepsPerSecond = 1; //limit min speed
		if(stepsPerSecond > 2000) stepsPerSecond = 2000; //limit max speed
		timer->Instance->ARR = (uint16_t)(65535.0/stepsPerSecond); //update timer
	}
	void setTargetAngle(float t){targetAngle = t;}
	void update(){
		encoderPos = encoder->position();
		error= targetAngle-encoderPos; //feedback

		float vel = pgain*error; //proportional term

		//set velocity
		stepper->setDir(vel > 0);
		float stepsPerSecond = abs(vel);
		if (abs(error) > 0.5) stepper->step(); //dont step if below min
		setTimer(stepsPerSecond);
	}
};


#endif /* MOTORCONTROL_H_ */
