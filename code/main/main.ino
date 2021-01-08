enum contrStatus {RUNNING, CLOCK_ADAPT_HOUR, CLOCK_ADAPT_MIN};
enum contrStatus C_STATUS;

int NOW_HOUR;
int NOW_MIN;
int NOW_SEC;
int LAST_HOUR =- 1;
int LAST_MIN = -1;

int LED_BRIGHTNESS = 255;  // brigness of clock --> 0-255

void setup () {

  Serial.begin(9600);
  while(!Serial);
  DCF_init();
  CS_init();
  RTC_init();
  LED_init();
  DCF_searchInitTime();
}

void loop () {
//  DCF_run();
  CS_checkButtons();
  
  // update only when in Running mode
  if(C_STATUS == RUNNING){
    RTC_updateDateTime();
    LED_showCurrentTime();
  }
  
  delay(50);

}
