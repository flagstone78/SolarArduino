Stepper::Stepper(int _stepPin, int _dirPin) {
  dirPin = _dirPin;
  stepPin = _stepPin;
  pinMode(dirPin, OUTPUT);    // configures pin 3 (dir) to be an output
  digitalWrite(dirPin, dirState); // output voltage set to 3 volts/low 
  pinMode(stepPin, OUTPUT);    // configure pin 4 (pul) to be an output
  digitalWrite(stepPin, stepState);  // output voltage set to 3 volts/low
}

void Stepper::nextStep() {
  pulState = !pulState;
  digitalWrite(stepPin, pulState);
}

void Stepper::setDirection(bool direction) {
  dirState = direction;
  digitalWrite(dirPin, dirState);
}

void Stepper::switchDirection() {
  setDirection(!dirState);
}