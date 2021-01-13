void LED_showMinute() {
  if (NOW_MIN != LAST_MIN) {
    LAST_MIN = NOW_MIN;
    int mod_minute = NOW_MIN % 5;
    LED_setMinute(mod_minute);
  }
}

void LED_showHour() {
  int tempHour = NOW_HOUR;
  // adjust the minutes only in RUNNING mode
  if (C_STATUS == RUNNING) {
    if (NOW_MIN >= 25) {
      tempHour += 1;
    }
  }
  if (tempHour > 12) {
    tempHour = tempHour - 12;
  }
  if (tempHour == 12) {
    tempHour = 0;
  }
  // function call of setting the LEDS for the hours
  if (C_STATUS != RUNNING) {
    LED_setHour(tempHour);
  }
  else {
    if (tempHour != LAST_HOUR) {
      LAST_HOUR = tempHour;
      if(C_STATUS == RUNNING){LED_matrixScreen();}
      LED_setHour();
    }
  }
}

void LED_init () {
  FastLED.addLeds<WS2811, LED_PIN, GRB>(LEDs, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(LED_BRIGHTNESS);
  LED_initRoutine();
  LED_Blackout();
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
}

// check current time and set the LEDs
void LED_showCurrentTime() {
  
  LED_showESIST();
  LED_showMinute();
  LED_showHour();

  //  LED_showStrip();

}
