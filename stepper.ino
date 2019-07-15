Stepper::Stepper(int _enPin, int _dirPin, int _stepPin, float _radiansPerStep, float _maxRad, float _minRad, bool _reverse) 
  :radiansPerStep(_radiansPerStep), dirPin(_dirPin), stepPin(_stepPin), enPin(_enPin), reverse(_reverse)
{  
  maxStep = getStepAtAngle(_maxRad);
  minStep = getStepAtAngle(_minRad);
  pinMode(dirPin, OUTPUT);    // configures pin 3 (dir) to be an output
  digitalWrite(dirPin, dirState); // output voltage set to 3 volts/low 
  pinMode(stepPin, OUTPUT);    // configure pin 4 (pul) to be an output
  digitalWrite(stepPin, stepState);  // output voltage set to 3 volts/low
  currentStep = 0;

  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, 0);  // output voltage set to 3 volts/low
}

void Stepper::nextStep() {
  //increment current step
  if (dirState) {
    if(currentStep < maxStep){
      currentStep += 1;
      stepState = !stepState;
      digitalWrite(stepPin, stepState);
    }
  } else {
    if(currentStep > minStep){
      currentStep -= 1;
      stepState = !stepState;
      digitalWrite(stepPin, stepState);
    }
  }
}

//no soft limits;
void Stepper::blindStep(){
  if (dirState) {
      currentStep += 1;
      stepState = !stepState;
      digitalWrite(stepPin, stepState);
  } else {
      currentStep -= 1;
      stepState = !stepState;
      digitalWrite(stepPin, stepState);
  }
}

uint32_t Stepper::getStepAtAngle(float rad){
  return (uint32_t) (rad/radiansPerStep);
}

void Stepper::setCurrentAngleTo(float rad) {
  //int step = rad/radiansPerStep;
  currentStep = getStepAtAngle(rad);//step;
}


float Stepper::getCurrentAngle() {
  return (float) currentStep * radiansPerStep;
}

void Stepper::setDirection(bool direction) {
  dirState = direction;
  digitalWrite(dirPin, dirState^reverse); //reverses direction if reverse is true
}

bool Stepper::getDirection() {
  return dirState;
}

void Stepper::switchDirection() {
  setDirection(!dirState);
}

void Stepper::printStatus(){
  Serial.print("Dir: ");
  Serial.print(dirState);
  Serial.print("  ");
  Serial.print("MinStep: ");
  Serial.print(minStep);
  Serial.print("  ");
  Serial.print(currentStep > minStep);
  Serial.print("  ");
  Serial.print("MaxStep: ");
  Serial.print(maxStep);
  Serial.print("  ");
  Serial.print(currentStep < maxStep);
  Serial.print("  ");
  Serial.print("Angle: ");
  Serial.print(getCurrentAngle()*180.0/PI);
  Serial.print("  ");
  Serial.print(currentStep);
  Serial.print("/");
  Serial.print(getStepAtAngle(2*PI));
  Serial.print("\n");
}
