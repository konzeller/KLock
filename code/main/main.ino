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
    DCF_getTime();
    RTC_updateDateTime();
    LED_showCurrentTime();
  }

  delay(50);

}
