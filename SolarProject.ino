/*
 Name:		SolarProject.ino
 Created:	5/28/2018 6:30:33 PM
 Author:	Alan
*/

#include <Wire.h>
#include "header.h"


const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

const int en = 2;
const int dir = 3;
const int pul = 4;

bool pulState = 0;

const float longitude = 44.0121; //Longitude of the solar collector, needs to be changed for current location, current Rochester Mn
const float latitude = 92.4802; //Latitude of the solar collector, needs to be changed for current location, rochester Mn

const float slope = 0; //platform angle relative to ground

long int nextTime;
int delayTime;
String readString;


Rtc* clock;
Accel* accel;
Compass* compass;

void setup() {
  Wire.begin();
  clock = new Rtc();
  accel = new Accel();
  compass = new Compass();

  //stepper motor
  pinMode(en, OUTPUT);
  digitalWrite(en, LOW);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, HIGH);
  pinMode(pul, OUTPUT);
  digitalWrite(pul, LOW);

  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("starting serial");

  //timer
  nextTime = millis() + 1000;
  delayTime = 0;
}

void loop() {
  if (millis()>nextTime) {
    if (Serial.available() >= 3) {
      delayTime = Serial.parseInt();
    }

    //stepper motor
    pulState = !pulState;
    digitalWrite(pul, pulState);

    clock->printTime();
    accel->printAccel();
    compass->printDirection();

    //timer
    Serial.print("delay: ");
    Serial.print(delayTime);
    Serial.print("\r\n");
    nextTime = millis() + (1000 - delayTime);
  }
}
