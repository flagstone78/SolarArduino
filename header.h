#ifndef HEADER
#define HEADER

#include "RTCLib.h"

//addresses
#define ACCELEROMETER_ADDRESS 0x69 // I2C address of the MPU-6050 accelerometer
#define COMPASS_ADDRESS 0x1E //0011110b, I2C 7bit address of HMC5883
#define DS3231_I2C_ADDRESS 0x68
#define DS3231_I2C_EEPROM 0x57

class Accel{
  int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
  public:
  Accel();
  float getZenith(); // angle for from vertical line straight up
  void printAccel();
};

class Compass{
  int CompX,CompY,CompZ;
  public:
  Compass();
  float getAzimuth();  // Degrees west of due south
  void printDirection();
};

class Rtc{
  RTC_DS3231 t;
  int16_t second, minute, hour, day, month, year;
  DateTime currentTime;
  public:
  Rtc();
  
  bool isLeapYear(uint16_t year);
  uint16_t getDayOfYear(uint16_t year, uint8_t month, uint8_t day);
  
  float julianDay();
  void printTime();
};

struct target{
  float azimuth = 0;
  float zenith = 0;
};

#endif // !HEADER
