#define UPDATE_DCF_AT_HOUR 20  // update daily at 3:30 AM
#define UPDATE_DCF_AT_MIN 0 // update daily at 3:30 AM

enum contrStatus {RUNNING, INIT_PHASE, CLOCK_ADAPT_HOUR, CLOCK_ADAPT_MIN};
enum contrStatus C_STATUS;

int NOW_YEAR;
int NOW_MONTH;
int NOW_DAY;
int NOW_HOUR;
int NOW_MIN;
int NOW_SEC;
int LAST_HOUR = - 1;
int LAST_MIN = -1;

bool AM_PM_ON = false;

int LED_BRIGHTNESS = 255;  // brigness of clock --> 0-255

void setup () {

  Serial.begin(9600);
//  while (!Serial);
  C_STATUS = INIT_PHASE;
  DCF_init();
  LED_init();
  CS_init();
  RTC_init();

  DCF_searchInitTime();
  C_STATUS = RUNNING;
}

void loop () {
  CS_checkButtons();

  // update only when in Running mode
  if (C_STATUS == RUNNING) {
    if((NOW_HOUR == UPDATE_DCF_AT_HOUR) & (NOW_MIN == UPDATE_DCF_AT_MIN)){
       DCF_searchInitTime();
    }
    RTC_updateDateTime();
    LED_showCurrentTime();
  }

  delay(50);

}
