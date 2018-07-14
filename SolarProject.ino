/*
 Name:		SolarProject.ino
 Created:	5/28/2018 6:30:33 PM
 Author:	Alan Mathias
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
  Wire.begin();            // Initialte teh wire library and join the I2c bus as a master or slave
  clock = new Rtc();       // set up for clock
  accel = new Accel();     // set up acceleromter 
  compass = new Compass(); // set up compass

  //stepper motor
  pinMode(en, OUTPUT);     //configures pin 2 (en) to be an output
  digitalWrite(en, LOW);   // output voltage set to 5 volts/high 
  pinMode(dir, OUTPUT);    // configures pin 3 (dir) to be an output
  digitalWrite(dir, HIGH); // output voltage set to 3 volts/low 
  pinMode(pul, OUTPUT);    // configure pin 4 (pul) to be an output
  digitalWrite(pul, LOW);  // output voltage set to 3 volts/low

  // initialize the serial port:
  Serial.begin(9600);      // sets data rate to 9600 bits per second
  Serial.println("starting serial");  // prints data to the serial port as human readable ASCII text

  //timer
  nextTime = millis() + 1000;  //sets next time to process - millis number of millisecond s sinde the arduino board begain running + 1000 
  delayTime = 0;               // set delay time
}

void loop() {                             // start to for controlling the solar tracker
  if (millis()>nextTime) {                // process every pet the frequency that nexTime is set to above 
    if (Serial.available() >= 3) {        // if the number of bytes available for reading is >= 3 then determines the delay time 
      delayTime = Serial.parseInt();      // reads delay time 
    }

    //stepper motor
    pulState = !pulState;           
    digitalWrite(pul, pulState);    // write pulState to pin 4 (pul) of the stepper motor  

    //clock->printTime();             // call subroutine to print the time
    //accel->printAccel();            // call subroutine to print the accelorometer values
    accel->getZenith();            // call subroutine to print the accelorometer position
    //compass->printDirection();      // call subroutine to print the angular direction of the compass
    compass->getAzimuth();

    //timer
    Serial.print("delay: ");        // print the work delay to the screen 
    Serial.print(delayTime);        // print the delay time time to the screen
    Serial.print("\r\n");           // print text after delay time printed
    nextTime = millis() + (1000 - delayTime);   //update nextTime including the delay time obtained above
  }
}
