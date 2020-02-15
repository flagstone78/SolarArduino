//Optical encoder class
//For arduino nano, pins d2 and d3 must be used for interrupts
#define CHAPIN 2
#define CHBPIN 3

void gCHA(){
  if(azimuthEncoder){
    azimuthEncoder->CHA();
  }
}

void gCHB(){
  if(azimuthEncoder){
    azimuthEncoder->CHB();
  }
}

void OpticalEncoder::CHA(){
  bool a = digitalRead(CHAPIN);
  bool b = digitalRead(CHBPIN);

  if(a^b){
    currentCount++;
  } else {
    currentCount--;
  }
  currentCount = currentCount%totalCount;
}

void OpticalEncoder::CHB(){
  bool a = digitalRead(CHAPIN);
  bool b = digitalRead(CHBPIN);

  if(a^b){
    currentCount--;
  } else {
    currentCount++;
  }
  currentCount = currentCount%totalCount;
}


OpticalEncoder::OpticalEncoder(int chi, int16_t initCount, int16_t totalCount, bool EncoderInvert): 
chi(chi), // Channel index pin
currentCount(initCount), //position to be set when the index is triggered
totalCount(totalCount),
encoderInvert(EncoderInvert){ //flips direction of encoder in software
  //set pin modes
  pinMode(CHAPIN, INPUT_PULLUP);
  pinMode(CHBPIN, INPUT_PULLUP);
  pinMode(chi, INPUT);

  //set interrupts
  attachInterrupt(digitalPinToInterrupt(CHAPIN),gCHA,CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHBPIN),gCHB,CHANGE);

  //constrain current count
  currentCount = currentCount%totalCount;
}

bool OpticalEncoder::getIndex(){
  return digitalRead(chi);
}

uint16_t OpticalEncoder::getPosition(){
  return currentCount;
}

//convert raw value into angle (radians)
float OpticalEncoder::getAngle(){
  return 2*PI*getPosition()/totalCount;
}

//convert angle to raw value (radians)
void OpticalEncoder::setAngle(float angle){
  angle -= ((int)(angle/2*PI))*2*PI;
  currentCount = angle*totalCount/(2*PI);
}
