#include <Wire.h>
#include "DCF77.h" //Alle DCF77 Funktionen laufen ueber diese Bibliothek.
#include "TimeLib.h"


#define DCF77PIN 7
#define FUNKCYCLE 1000

DCF77 DCF = DCF77(DCF77PIN, 0, true);
bool DCF_stopped = true;
unsigned char signal = 0;
unsigned char buffer;
time_t DCFtime;

void DCF_init() {
  pinMode(DCF77PIN, INPUT);

  DCF.Start();
  Serial.println("Waiting for DCF77 time ... ");
  Serial.println("It will take at least 2 minutes before a first time update.");
}

void DCF_searchInitTime() {
  int counter = 0;
  bool goOn = true;
  while (goOn)
  {
    delay(FUNKCYCLE);
    DCFtime = DCF.getTime();
    LED_blinkFUNK(FUNKCYCLE);
    if (DCFtime != 0) {
      DCF_setTime();
      Serial.println("NEW DCF Time available");
      goOn = false;
    }
    if (counter > 240) /*waiting for ~4 Minutes before time update cancled*/ {
      goOn = false;
      Serial.println("ERROR - NO DCF UPDATE available");
    }
    counter++;
  }

}

//void DCF_getTime() {
//  DCFtime = DCF.getTime();
//  if (DCFtime != 0) {
//    Serial.println("NEW DCF Time available");
//    DCF_setTime();
//  }
//}

void DCF_setTime() {
  NOW_YEAR = year(DCFtime);
  NOW_MONTH = month(DCFtime);
  NOW_DAY = day(DCFtime);
  NOW_HOUR = hour(DCFtime);
  NOW_MIN = minute(DCFtime);
  NOW_SEC = second(DCFtime);
  RTC_setNewTime();
}
