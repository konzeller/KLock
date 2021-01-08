#include "RTClib.h"

/* Define default Time values */
int DEFAULT_YEAR = 2021;
int DEFAULT_MONTH = 5;
int DEFAULT_DAY = 14;
int DEFAULT_HOUR = 20;
int DEFAULT_MIN = 11;
int DEFAULT_SEC = 0;

//char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

RTC_DS3231 RTC;
DateTime RTC_NOW;


void RTC_init(){
  if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 
  if (RTC.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time with default values
    RTC.adjust(DateTime(DEFAULT_YEAR,DEFAULT_MONTH,DEFAULT_DAY,DEFAULT_HOUR,DEFAULT_MIN,DEFAULT_SEC));
    // This line sets the RTC with an explicit date & time, for example to set
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}


void RTC_updateDateTime(){
  RTC_NOW = RTC.now();

  NOW_HOUR = RTC_NOW.hour();
  NOW_MIN = RTC_NOW.minute();
  NOW_SEC = RTC_NOW.second();
  }


void RTC_setNewTime(int new_hour, int new_minute){
  Serial.println("");
  Serial.print("NEW TIME saved: ");
  Serial.print(new_hour,DEC);
  Serial.print(new_minute,DEC);
  
  RTC.adjust(DateTime(DEFAULT_YEAR,DEFAULT_MONTH,DEFAULT_DAY,new_hour,new_minute,(int)second()));
}
