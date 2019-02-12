Stepper::Stepper(int _enPin, int _dirPin, int _stepPin, int _stepsPerRevolution) 
  :stepsPerRevolution(_stepsPerRevolution), dirPin(_dirPin), stepPin(_stepPin), enPin(_enPin)
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
  if (currentStep < 0) {
    currentStep = stepsPerRevolution;
  } else if (currentStep > stepsPerRevolution) {
    currentStep = 0;
  }

  digitalWrite(stepPin, stepState);
}

void Stepper::setCurrentAngleTo(float angle) {
  int step = stepsPerRevolution*(angle / 6.283185307179586476925286766559);
  currentStep = step;
}

float Stepper::getCurrentAngle() {
  return 6.283185307179586476925286766559 * (float) currentStep / (float) stepsPerRevolution;
}

void Stepper::setDirection(bool direction) {
  dirState = direction;
  digitalWrite(dirPin, dirState);
}

void Stepper::switchDirection() {
  setDirection(!dirState);
}
