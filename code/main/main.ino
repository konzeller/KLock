

//#include "DCF77.h" //Alle DCF77 Funktionen laufen ueber diese Bibliothek.
//#include "TimeLib.h"

//#define DCF77PIN 9

enum contrStatus {RUNNING, CLOCK_ADAPT_HOUR, CLOCK_ADAPT_MIN};
enum contrStatus C_STATUS;

int NOW_HOUR;
int NOW_MIN;
int NOW_SEC;
int LAST_HOUR =- 1;
int LAST_MIN = -1;

int LED_BRIGHTNESS = 255;  // brigness of clock --> 0-255


//DCF77 DCF = DCF77(DCF77PIN,0);

void setup () {

  Serial.begin(9600);
//
  CS_init();
  RTC_init();
  LED_init();

//  pinMode(DCF77PIN, INPUT);
//  digitalWrite(LED_PIN, LOW);
}
 
void loop () {
  
  CS_checkButtons();
  
  // update only when in Running mode
  if(C_STATUS == RUNNING){
    RTC_updateDateTime();
    LED_showCurrentTime();
  }
  
  delay(50);
//  Serial.println(C_STATUS);

}
