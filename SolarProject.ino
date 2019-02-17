/*
 Name:		SolarProject.ino
 Created:	5/28/2018 6:30:33 PM
 Author:	Alan Mathias
*/

#include <Wire.h> 
#include "header.h"


const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution

Stepper azimuthMotor(5, 6, 7, 6400);
Stepper elevationMotor(2, 3, 4, 6400);
LimitSwitch azimuthSwitch(12, true);
LimitSwitch elevationSwitch(13, true);

Rtc* clock;
Accel* accel;
//Compass* compass;


const float longitude = -92.4802; //Longitude of the solar collector, needs to be changed for current location, current Rochester Mn
const float latitude = 44.0121; //Latitude of the solar collector, needs to be changed for current location, rochester Mn

const float slope = 0; //platform angle relative to ground

long int nextTime;
int delayTime;
String readString;




//unsigned int speed = 400;
//int increment = 1;

enum STATE{CALIBRATE,TRACKING, TEST};
STATE curState = CALIBRATE;

void setup() {
  Wire.begin();            // Initialte the wire library and join the I2c bus as a master or slave
  clock = new Rtc();       // set up for clock
  accel = new Accel();     // set up acceleromter 
  //compass = new Compass(); // set up compass

  //compassMotor.setCurrentAngleTo(1.5707963267);

  // initialize the serial port:
  Serial.begin(115200);      // sets data rate to 9600 bits per second
  Serial.println("starting serial");  // prints data to the serial port as human readable ASCII text

  //timer
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 32000; //compare match register 31250 is 1 second
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);    // 256 prescaler 
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt

  nextTime = millis();  //sets next time to process - millis number of millisecond s sinde the arduino board begain running + 1000 
  delayTime = 999;               // set delay time
}

SIGNAL(TIMER1_COMPA_vect)
{
  //azimuthMotor.nextStep();
  //elevationMotor.nextStep();
}

void updateOCR1A(int val) {
  OCR1A = val;
  if (TCNT1>val) {
    TCNT1 = 0;
    //azimuthMotor.nextStep();
    //elevationMotor.nextStep();
  }
}

float angle = 1.5707963267;
float targetAngle = 1.5707963267;
float diff = 0;

float compassAngle = 1.5707963267;
float targetCompass = 1.5707963267;
float compassDiff = 0;

void loop() {                             // start to for controlling the solar tracker
  switch(curState){
  case CALIBRATE:
    azimuthMotor.setDirection(0); //test
    while(!azimuthSwitch.pressed()){
      azimuthMotor.nextStep();
      delay(20);
    }
    azimuthMotor.setCurrentAngleTo(55.0); //set to angle of the limit switch //55 degrees from north
    Serial.println("Done calibrating");
    curState = TEST;
    break;
  case TRACKING:
    if (millis()>nextTime) {                // process every step the frequency that nexTime is set to above 
      if (Serial.available() >= 3) {        // if the number of bytes available for reading is >= 3 then determines the delay time 
        delayTime = Serial.parseInt();      // reads delay time 
      }
  
      //clock->printTime();             // call subroutine to print the time
      //accel->printAccel();            // call subroutine to print the accelorometer values
      angle = accel->getZenith();            // call subroutine to print the accelorometer position
      elevationMotor.setDirection((angle < targetAngle));
      
      /*diff = angle - targetAngle;
      if (diff < 0) { diff *= -1.0; }
      if(diff > .25){
        speed = (10000.0 / diff);
      } else {
        speed = 40000;//
      }*/
      
      //compass->printDirection();      // call subroutine to print the angular direction of the compass
      compassAngle = azimuthMotor.getCurrentAngle();
      Serial.print("C");
      Serial.println(compassAngle * 180.0 / 3.1415926535897932384626433832795);
      azimuthMotor.setDirection(compassAngle < targetCompass);
  
      //timer
      //Serial.print("delay: ");        // print the work delay to the screen 
      //Serial.print(delayTime);        // print the delay time time to the screen
      //Serial.print("\r\n");           // print text after delay time printed
      nextTime = millis() + (1000 - delayTime);   //update nextTime including the delay time obtained above
    }
    break;
  default:
    break;
  }
}
