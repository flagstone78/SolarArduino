#include "header.h"

Rtc::Rtc(){
  t = RTC_DS3231();
  t.begin();
  DateTime set(2018, 6, 16, 14, 33, 0);
  t.adjust(set);
  second = 0; 
  minute = 0; 
  hour = 0;
  day = 0;
  month = 0;
  year = 0;
}

bool Rtc::isLeapYear(uint16_t year){
  if(year%4 != 0) return false;
  if(year%100 == 0) return (year%400 == 0);
  return true;
}

uint16_t Rtc::getDayOfYear(uint16_t year, uint8_t month,uint8_t day){
  uint16_t days[12] = {0,31,59,90,120,151,181,212,243,273,304,334}; //days since beginning of year for each month
  return days[month-1] + day + ((month>2)*isLeapYear(year));
}

void Rtc::printTime() {
  //currentTime = t.now();

  second = currentTime.second();
  minute = currentTime.minute();
  hour = currentTime.hour();
  month = currentTime.month();
  year = currentTime.year();
  day = currentTime.day();

  Serial.print("RTC: ");
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute < 10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second < 10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(day, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print("    JulianDay: ");
  Serial.println(getDayOfYear(year,month,day));
}
