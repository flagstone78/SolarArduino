Stepper::Stepper(int _enPin, int _dirPin, int _stepPin, double _radiansPerStep) 
  :radiansPerStep(_radiansPerStep), dirPin(_dirPin), stepPin(_stepPin), enPin(_enPin)
{  
  pinMode(dirPin, OUTPUT);    // configures pin 3 (dir) to be an output
  digitalWrite(dirPin, dirState); // output voltage set to 3 volts/low 
  pinMode(stepPin, OUTPUT);    // configure pin 4 (pul) to be an output
  digitalWrite(stepPin, stepState);  // output voltage set to 3 volts/low
  currentStep = 0;

  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, 0);  // output voltage set to 3 volts/low
}

void Stepper::nextStep() {
  stepState = !stepState;
  
  //increment current step
  if (dirState) {
    currentStep += 1;
  } else {
    currentStep -= 1;
  }

  //wrap current step
  /*if (currentStep < 0) {
    currentStep = stepsPerRevolution;
  } else if (currentStep > stepsPerRevolution) {
    currentStep = 0;
  }*/

  digitalWrite(stepPin, stepState);
}

void Stepper::setCurrentAngleTo(float rad) {
  int step = rad/radiansPerStep;
  currentStep = step;
}

float Stepper::getCurrentAngle() {
  return (float) currentStep * radiansPerStep;
}

void Stepper::setDirection(bool direction) {
  dirState = direction;
  digitalWrite(dirPin, dirState);
}

bool Stepper::getDirection() {
  return dirState;
}

void Stepper::switchDirection() {
  setDirection(!dirState);
}
