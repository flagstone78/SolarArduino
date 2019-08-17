#include "header.h"

Rtc::Rtc(){
  t = RTC_DS3231();     // set clock type installed 
  t.begin();            // code to set the time reported by the digital clock
  
  // uncomment the next to lines to set the time for the clock  ???
  //DateTime set(2019, 6, 22, 14, 9, 0);
  //t.adjust(set);
  second = 0; 
  minute = 0; 
  hour = 0;
  day = 0;
  month = 0;
  year = 0;

  testTime = DateTime(2019,7,15,2,0,0);
}

bool Rtc::isLeapYear(uint16_t year){       // determine if it is leap year or not
  if(year%4 != 0) return false;
  if(year%100 == 0) return (year%400 == 0);
  return true;
}

uint16_t Rtc::getDayOfYear(uint16_t year, uint8_t month,uint8_t day){     // determine number of the days from the beginning of the year, julian day in solar book
  uint16_t days[12] = {0,31,59,90,120,151,181,212,243,273,304,334}; //days since beginning of year for each month
  return days[month-1] + day + ((month>2)*isLeapYear(year));
}

void Rtc::printTime() {
  currentTime = t.now();
  //currentTime = testTime;
  
  second = currentTime.second();  //determine year, month, day, hour, minute, second from the real time clock 
  minute = currentTime.minute();
  hour = currentTime.hour();
  month = currentTime.month();
  year = currentTime.year();
  day = currentTime.day();

  Serial.print("RTC: ");    // print time to the screen from the data gathered
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
  Serial.print("    Seconds2000: ");
  Serial.println(currentTime.secondstime());
  //Serial.print("    JulianDay: ");
  //Serial.println(getDayOfYear(year,month,day));
}

uint32_t Rtc::seconds(){
  uint32_t offset = 0; //(uint32_t)13*60*60 + 34*60;
  return t.now().secondstime()- offset;
  //return testTime.secondstime();
}

void Rtc::setTime(){
  DateTime newtime(2019,5,8,11,35,0);
  t.adjust(newtime);
}
