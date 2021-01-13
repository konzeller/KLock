#include <Wire.h>

#define BTN_PIN_CLOCKADAPT 6
#define BTN_PIN_SET 8


bool BTN_CLOCKADAPT_LAST = true;
bool BTN_SET_LAST = true;

void CS_init() {
  C_STATUS = RUNNING;
  pinMode(BTN_PIN_CLOCKADAPT, INPUT_PULLUP);
  //  pinMode(BTN_PIN_HOUR, INPUT_PULLUP);
  pinMode(BTN_PIN_SET, INPUT_PULLUP);
}


// check if and which button is klicked
void CS_checkButtons() {
  switch (C_STATUS) {
    case RUNNING:
      CS_checkBRIGHT_BTN();
      CS_checkCLOCKADAPT_BTN();
      break;

    case CLOCK_ADAPT_HOUR:
      CS_checkCLOCKADAPT_BTN();
      CS_checkSET_TIME_BTN();
      break;

    case CLOCK_ADAPT_MIN:
      CS_checkCLOCKADAPT_BTN();
      CS_checkSET_TIME_BTN();
      break;
  }
  //  Serial.println(val);

}

void CS_checkBRIGHT_BTN() {
  if ((digitalRead(BTN_PIN_SET) == false)) {
    CS_changeBrightness();
    delay(30);
  }
}

void CS_changeBrightness() {
  LED_BRIGHTNESS = LED_BRIGHTNESS + 5;
  if (LED_BRIGHTNESS <= 255) {
    LED_setBrightness();
  } else {
    LED_blinkALL(2);
    LED_BRIGHTNESS = 0;
    LED_showCurrentTime();
  }
}

// check in which running status the programm is
void CS_checkCLOCKADAPT_BTN() {
  bool btn_val = digitalRead(BTN_PIN_CLOCKADAPT);
  if (btn_val == LOW) {
    if (btn_val != BTN_CLOCKADAPT_LAST) {
      switch (C_STATUS) {
        case RUNNING:
          // here only the current hour should be shown in different colour
          LED_Blackout();
          LED_showHour();
          C_STATUS = CLOCK_ADAPT_HOUR;
          LED_showStrip();
          break;
        case CLOCK_ADAPT_HOUR:
          C_STATUS = CLOCK_ADAPT_MIN;
          // here only the current minute should be shown in different colour
          LED_Blackout();
          LED_showMinute();
          //            LED_showStrip();
          break;
        case CLOCK_ADAPT_MIN:
          RTC_setNewTime();//NOW_HOUR, NOW_MIN);
          C_STATUS = RUNNING;
          LED_blinkALL(1);
          break;
      }
    }
  }
  BTN_CLOCKADAPT_LAST = btn_val;
}

void CS_checkSET_TIME_BTN() {
  bool btn_val = digitalRead(BTN_PIN_SET);
  if (btn_val == LOW) {
    if (btn_val != BTN_SET_LAST) {
      switch (C_STATUS) {
        case CLOCK_ADAPT_HOUR:
          Serial.println("HOUR PLUS ONE");
          CS_adaptHour();
          // here only the current minute should be shown in different colour
          break;
        case CLOCK_ADAPT_MIN:
          Serial.println("MINUTE PLUS ONE");
          CS_adaptMinute();
          break;
      }
    }
  }
  BTN_SET_LAST = btn_val;
}

// adapt the hours
void CS_adaptHour() {
  NOW_HOUR++;
  if (NOW_HOUR == 24) {
    NOW_HOUR = 0;
  }
  LED_showHour();
  LED_showStrip();
}

// adapt the minutes
void CS_adaptMinute() {
  NOW_MIN++;
  if (NOW_MIN == 60) {
    NOW_MIN = 0;
  }
  LED_showMinute();
  //  LED_showStrip();
}
