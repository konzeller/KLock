#define UPDATE_DCF_AT_HOUR 3  // update daily at 3:30 AM
#define UPDATE_DCF_AT_MIN 30 // update daily at 3:30 AM

enum contrStatus {RUNNING, INIT_PHASE, CLOCK_ADAPT_HOUR, CLOCK_ADAPT_MIN/*, CLED_SETCOLOR*/};
enum contrStatus C_STATUS;

int8_t NOW_YEAR;
int8_t NOW_MONTH;
int8_t NOW_DAY;
int8_t NOW_HOUR;
int8_t NOW_MIN;
int8_t NOW_SEC;
int8_t LAST_HOUR = - 1;
int8_t LAST_MIN = -1;

bool AM_PM_ON = false;

uint8_t LED_BRIGHTNESS = 180;  // brigness of clock --> 0-255


void setup () {

  Serial.begin(9600);
  //  while (!Serial);
  C_STATUS = INIT_PHASE;
  DCF_init();
  LED_init();
  LDR_init();
  CS_init();
//    RTC_init();
  if (RTC_init()) {
    Serial.println("SearchInitTime");
    DCF_searchInitTime();
  }
  C_STATUS = RUNNING;
}

void loop () {
  CS_checkButtons();

  // update only when in Running mode
  if (C_STATUS == RUNNING) {
    if ((NOW_HOUR == UPDATE_DCF_AT_HOUR) & (NOW_MIN == UPDATE_DCF_AT_MIN)) {
      DCF_searchInitTime();
    }
    RTC_updateDateTime();
    LED_showCurrentTime();
  }
  LDR_read();

  delay(50);

}
