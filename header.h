#ifndef HEADER
#define HEADER

#include "RTCLib.h"
#include <math.h>

//addresses
#define ACCELEROMETER_ADDRESS 0x69 // I2C address of the MPU-6050 accelerometer
#define COMPASS_ADDRESS 0x1E //0011110b, I2C 7bit address of HMC5883
#define DS3231_I2C_ADDRESS 0x68 // clock
#define DS3231_I2C_EEPROM 0x57  // clock


struct xyz {
  float x; // reference x for Accelerometer verticle to the ground
  float y; // reference y for Accelerometer verticle to the ground
  float z; // reference z for Accelerometer verticle to the ground

  xyz(float x = 0, float y = 0, float z = 0) :x(x), y(y), z(z) {}

  //float x2 = 0; // x for the current angle of the accelerometer
  //float y2 = 0; // y for the current angle of the accelerometer
  //float z2 = 0; // z for the current angle of the accelerometer
};

xyz cross(xyz v1, xyz v2) {
  // take the cross producct of xyz1 and xyz2
  xyz cross(
    (v1.y * v2.z - v1.z * v2.y),
    (v1.x * v2.z - v1.z * v2.x),
    (v1.x * v2.y - v1.y * v2.x)
  );
  return cross;
}

float dot(xyz v1, xyz v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

float magnitude(xyz v1) {
  return sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
}

class Accel{
  xyz ac;
  void takeAccel(); 
  const float offset;
  public:
  Accel(float _offset);
  float getZenith(); // angle for from vertical line straight up
  void printAccel();
};

class Compass{
  xyz comp;
  void takeMeasurement();
  public:
  Compass();
  float getAzimuth();  // Degrees west of due south
  void printDirection();
};

class Rtc{
  RTC_DS3231 t;
  int16_t second, minute, hour, day, month, year;
  DateTime currentTime;
  DateTime testTime;
  public:
  Rtc();
    
  bool isLeapYear(uint16_t year);
  uint16_t getDayOfYear(uint16_t year, uint8_t month, uint8_t day);
  
  float julianDay();
  void printTime();
  uint32_t seconds();

  void setTime();
};

struct target{
  float azimuth; //radians clockwise from north
  float elevation; //pi/2 degrees is up
};

target getTargetAzimuth(int day, float hours);

class Stepper {
  bool dirState = false;
  bool stepState = false;
  const int dirPin;
  const int stepPin;
  const int enPin;
  const bool reverse;

  const float radiansPerStep; //steps per revolution of the motor
  uint32_t maxStep; //maximum angle of motor
  uint32_t minStep; //minimum angle of motor
  uint32_t currentStep; //keeps track of the current step

public:
  Stepper(int _enPin, int _dirPin, int _stepPin, float _radiansPerStep, float _maxRad, float _minRad, bool _reverse);
  void nextStep();
  void blindStep();
  void setDirection(bool direction);
  bool getDirection();
  void switchDirection();
  void setCurrentAngleTo(float angle);
  float getCurrentAngle();
  uint32_t getStepAtAngle(float rad);
  void printStatus();
};

class LimitSwitch {
  const int pin;        //pin number that the switch is connected to
  const bool inverted;  //true if pressed is pulls pin low and released pulls pin high

public:
  LimitSwitch(int _pin, bool _inverted):pin(_pin),inverted(_inverted){pinMode(pin, INPUT_PULLUP);};
  bool pressed(){return (inverted^digitalRead(pin));};
};

class Encoder {
  const uint8_t addr;
  const uint16_t offset;
  const bool encoderInvert;
  uint8_t dataBuffer[2];
  public:
  Encoder(uint8_t address, int16_t Offset, bool EncoderInvert);
  uint16_t getPosition();
  float getAngle();
};

#endif // !HEADER
