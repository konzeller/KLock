#include <Wire.h>
#include "DCF77.h" //Alle DCF77 Funktionen laufen ueber diese Bibliothek.
#include "TimeLib.h"


#define DCF77PIN 7
#define FUNKCYCLE 1000

DCF77 DCF = DCF77(DCF77PIN, 0, true);
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
      //      RTC_setNewTime(hour(), minute());
      goOn = false;
    }
    if (counter > 240) {
      goOn = false;
      Serial.println("ERROR - NO DCF UPDATE available");
    }
    counter++;
  }

}

void DCF_setTime() {
  setTime(DCFtime);
  RTC_setNewTime((int)hour(), (int)minute());
}
