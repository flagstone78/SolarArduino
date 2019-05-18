/*
 Name:		SolarProject.ino
 Created:	5/28/2018 6:30:33 PM
 Author:	Alan Mathias
*/

#include <Wire.h> 
#include "header.h"

#define PI 3.1415926535897932384626433832795

const float longitude = -92.4802; //Longitude of the solar collector, needs to be changed for current location, current Rochester Mn
const float latitude = 44.0121; //Latitude of the solar collector, needs to be changed for current location, rochester Mn
const float timeZone = -5;

const float slope = 0; //platform angle relative to ground

float maxElevation = 120.0*PI/180.0;
float minElevation = 10.0*PI/180.0;

const float azimuthLimitLocation = 55.0; //hard stop limit switch location
float maxAzimuth = (320.0)*PI/180.0;
float minAzimuth = azimuthLimitLocation*PI/180.0;


const int stepsPerRevolution = 800;  // change this to fit the number of steps per revolution

float azimuthRadPerStep = (2*PI*9.0*.467)/(stepsPerRevolution*60.0); //2 pi radian * (9/60) gear ratio *3.5

Stepper azimuthMotor(5, 6, 7, azimuthRadPerStep, maxAzimuth, minAzimuth); // 54 degrees per revoloution of motor // (2 pi *9)/(3200*200*60)
Stepper elevationMotor(2, 3, 4, 15.0*PI/(stepsPerRevolution*360.0), maxElevation, minElevation); //  7.5 degrees elevation per motor revolotion
LimitSwitch azimuthSwitch(10, false);
LimitSwitch elevationSwitch(11, false);

Rtc* clock;
Accel* accel;
//Compass* compass;

long int nextTime;
int delayTime;
String readString;

//unsigned int speed = 400;
//int increment = 1;

enum STATE{CALIBRATE,TRACKING, TEST};
STATE curState = CALIBRATE;

void enableTimer1(){
  TIMSK1 |= (1 << OCIE1A); 
}

void disableTimer1(){
  TIMSK1 &= ~(1 << OCIE1A); 
}

volatile target Target;

volatile float elevationAngle = 1.5707963267;
//Target.elevation = ((90.0-1.2)*PI)/180.0; //0 degrees is at 7.56 degrees; 90 at 88.25
volatile float elevationDiff = 0;

volatile float azimuthAngle = 0;
//Target.azimuth = (0.0*PI)/180.0; //target angle
volatile float azimuthDiff = 0;

void setup() {
  Wire.begin();            // Initialte the wire library and join the I2c bus as a master or slave
  clock = new Rtc();       // set up for clock
  //clock->setTime();
  accel = new Accel();     // set up acceleromter 

  // initialize the serial port:
  Serial.begin(115200);      // sets data rate to 9600 bits per second
  Serial.println("starting serial");  // prints data to the serial port as human readable ASCII text

  //timer
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  OCR1A = 5025;//1025; //compare match register 31250 is 1 second
  TCCR1B |= (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  disableTimer1(); // timer compare interrupt. enabled further down

  nextTime = millis();  //sets next time to process - millis number of millisecond s since the arduino board begain running + 1000 
  delayTime = 999;               // set delay time
}

SIGNAL(TIMER1_COMPA_vect) //interrupt handler to move motors periodically
{
  elevationMotor.setDirection(0 > elevationDiff); //set direction of motor
  if(elevationDiff > .017 || elevationDiff < -.017){ //step if outside of +- 1 degree
    elevationMotor.nextStep(); //step in that direction
  }

  Serial.print(elevationAngle * 180.0 / PI);
  Serial.print("  ");
  Serial.print(Target.elevation * 180.0 / PI);
  Serial.print("  ");
  Serial.print(elevationDiff * 180.0 / PI);
  Serial.print("\n");
  
  
  azimuthMotor.setDirection(0 < azimuthDiff); //set direction
  if(azimuthDiff > .017 || azimuthDiff < -.017){ //step if outside of +- 1 degree
    azimuthMotor.nextStep(); //step in that direction
  }

  /*Serial.print(azimuthAngle * 180.0 / PI);
  Serial.print("  ");
  Serial.print(Target.azimuth * 180.0 / PI);
  Serial.print("  ");
  Serial.print(azimuthDiff * 180.0 / PI);
  //Serial.print("    ");
  //Serial.print(azimuthMotor.getDirection());
  Serial.print("\n");*/
}

void updateOCR1A(int val) {
  //OCR1A = val;
  if (TCNT1>val) {
    TCNT1 = 0;
    azimuthMotor.nextStep();
    elevationMotor.nextStep();
  }
}

void loop() {// start to for controlling the solar tracker
  switch(curState){
  case CALIBRATE:
    disableTimer1(); //stops motor tracking until the current position is known
    
    //curState = TEST;
    //break;
    
    azimuthMotor.setDirection(0);
    volatile int delayTime;
    while(!azimuthSwitch.pressed()){ //move motor to limit switch
      azimuthMotor.blindStep();
      Serial.println(azimuthSwitch.pressed());
      //azimuthMotor.printStatus();
      delayTime = 1600000000;
      while(delayTime > 0){ delayTime--;};
    }
    azimuthMotor.setCurrentAngleTo((float)azimuthLimitLocation*PI/180.0); //set to angle of the limit switch //55 degrees from north
    Serial.println("Done calibrating");
    Serial.print("Pointing to ");
    Serial.print(azimuthMotor.getCurrentAngle()*180.0/PI);
    Serial.print(" degrees\n ");
    //azimuthMotor.printStatus();
    //elevationMotor.printStatus();
    enableTimer1();
    
    curState = TRACKING;
    break;
  case TRACKING:
    //if (millis()>nextTime) {                // process every step the frequency that nexTime is set to above 
      //if (Serial.available() >= 3) {        // if the number of bytes available for reading is >= 3 then determines the delay time 
        //delayTime = Serial.parseInt();      // reads delay time 
      //}
      
      //clock->printTime();
      Target = getTargetAzimuth(clock->seconds());
      //azimuthMotor.printStatus();
      //Serial.print("Azimuth: ");
      //Serial.print(Target.azimuth*180/PI);
      //Serial.print("    Elevation: ");
      //Serial.println(Target.elevation*180/PI);
      
      elevationAngle = accel->getZenith(); // call subroutine to print the accelorometer position
      elevationDiff = Target.elevation - elevationAngle;
      elevationMotor.setCurrentAngleTo(elevationAngle);

      azimuthAngle = azimuthMotor.getCurrentAngle(); //get current azimuth
      azimuthDiff = Target.azimuth - azimuthAngle;
  
      //clock->printTime();             // call subroutine to print the time
      //accel->printAccel();            // call subroutine to print the accelorometer values
      //timer
      //Serial.print("delay: ");        // print the work delay to the screen 
      //Serial.print(delayTime);        // print the delay time time to the screen
      //Serial.print("\r\n");           // print text after delay time printed
      //nextTime = millis() + (1000 - delayTime);   //update nextTime including the delay time obtained above
    //}
    break;
  case TEST:
    /*for(int day = 0; day<1; day++){
      for(float seconds = 0; seconds<24*60*60; seconds+=1){
        target out = getTargetAzimuth(day, seconds);
        Serial.print("d: ");
        Serial.print(day);
        Serial.print("   ");
        Serial.print("h: ");
        Serial.print(seconds);
        Serial.print("   ");
        
        Serial.print("A: ");
        Serial.print(out.azimuth);
        Serial.print("   ");
        Serial.print("E: ");
        Serial.print(out.zenith);
        Serial.print('\n');
      }
    }*/
    break;
  default:
    break;
  }
}
