

//#include "DCF77.h" //Alle DCF77 Funktionen laufen ueber diese Bibliothek.
//#include "TimeLib.h"

//#define DCF77PIN 9

enum contrStatus {RUNNING, CLOCK_ADAPT, BRIGHTNESS};
enum contrStatus C_STATUS;

int NOW_HOUR;
int NOW_MIN;
int NOW_SEC;
int NEW_HOUR;
int NEW_MIN;
int NEW_SEC;
int LAST_HOUR =- 1;
int LAST_MIN = -1;


//DCF77 DCF = DCF77(DCF77PIN,0);

void setup () {
  
//#ifndef ESP8266
//  while (!Serial); // for Leonardo/Micro/Zero
//#endif
 
  Serial.begin(9600);
//
  CS_init();
  RTC_init();
  LED_init();

//  pinMode(DCF77PIN, INPUT);
//  digitalWrite(LED_PIN, LOW);
}
 
void loop () {
  
  RTC_updateDateTime();
  LED_showCurrentTime();
  CS_checkButtons();
  delay(50);
  Serial.println(C_STATUS);

}
