#include <Wire.h>
#include "FastLED.h"

#define LED_PIN 10
#define NUM_LEDS 114
#define FADE_VAL 100

uint32_t LED_COL_METEOR = 0xf8ff00;
uint32_t LED_COL_REGULAR = 0xffffff;
uint32_t LED_COL_CONFIG = 0xff0000;
uint32_t LED_COL_BLACK = 0x000000;
CRGB LED_CURRENT_COL(LED_COL_REGULAR);

CRGB LEDs[NUM_LEDS];

bool ALL_OUT = true;
bool ESIST_OFF = true;
bool AM_OFF = true;
bool PM_OFF = true;

int COLUMN[11][10] = {{10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
  {12, 13, 14, 15, 16, 17, 18, 19, 20, 21},
  {31, 30, 29, 28, 27, 26, 25, 24, 23, 22},
  {32, 33, 34, 35, 36, 37, 38, 39, 40, 41},
  {51, 50, 49, 48, 47, 46, 45, 44, 43, 42},
  {52, 53, 54, 55, 56, 57, 58, 59, 60, 61},
  {71, 70, 69, 68, 67, 66, 65, 64, 63, 62},
  {72, 73, 74, 75, 76, 77, 78, 79, 80, 81},
  {91, 90, 89, 88, 87, 86, 85, 84, 83, 82},
  {92, 93, 94, 95, 96, 97, 98, 99, 100, 101},
  {112, 111, 110, 109, 108, 107, 106, 105, 104, 103}
};

int FUNK[4] = {35, 48, 55, 68}; // FUNK
int UHR[3] = {82, 101, 103};
int AM[2] = {57, 66};
int PM[2] = {45, 58};

int ESIST[5]   = {10, 12, 32, 51, 52};        // static LED will not change

//int LED_MINUTES[44] = {72, 91, 92, 112, 9, 13, 30, 33, 50, 53, 70, 73, 90, 93, 111, 8, 14, 29, 34, 49, 54, 69, 74, 89, 94, 110, 49, 54, 69, 74, 89, 94, 110, 7, 15, 28, 75, 88, 95, 109, 6, 16, 27, 36};
int FUENF_NACH[8] = {72, 91, 92, 112, 75, 88, 95, 109};   // fünf nach
int ZEHN_NACH[8] = {9, 13, 30, 33, 75, 88, 95, 109};      // zehn vor oder nach
int ZWANZIG_NACH[11] = {50, 53, 70, 73, 90, 93, 111, 75, 88, 95, 109}; //zwanzig vor oder nach

//int DREIVIERTEL_F[11] = {8, 14, 29, 34, 49, 54, 69, 74, 89, 94, 110}; // dreiviertel
int VIERTEL_NACH[11] = {49, 54, 69, 74, 89, 94, 110, 75, 88, 95, 109}; // viertel
int FUENF_VOR_HALB[11] = {72, 91, 92, 112, 7, 15, 28, 6, 16, 27, 36};
int HALB[4] = {6, 16, 27, 36};    // halb
int FUENF_NACH_HALB[12] = {72, 91, 92, 112, 75, 88, 95, 109, 6, 16, 27, 36};
int FUENF_VOR[7] = {72, 91, 92, 112, 7, 15, 28};   // fünf nach
int ZEHN_VOR[7] = {9, 13, 30, 33, 7, 15, 28};      // zehn vor oder nach
int ZWANZIG_VOR[10] = {50, 53, 70, 73, 90, 93, 111, 7, 15, 28}; //zwanzig vor oder nach
int VIERTEL_VOR[10] = {49, 54, 69, 74, 89, 94, 110, 7, 15, 28}; // viertel

//int LED_HOURS[50] = {5, 17, 26, 3, 37, 77, 86, 97, 107, 4, 18, 25, 38, 78, 85, 98, 106, 76, 87, 96, 108, 3, 19, 24, 39, 44, 2, 20, 23, 40, 43, 60, 79, 84, 99, 105, 41, 42, 61, 62, 1, 21, 22, 56, 67, 63, 80, 83, 100, 104};
int EINS[4] = {5, 17, 26, 37};
int ZWEI[4] = {77, 86, 97, 107};
int DREI[4] = {4, 18, 25, 38};
int VIER[4] = {78, 85, 98, 106};
int FUENF[4] = {76, 87, 96, 108};
int SECHS[5] = {3, 19, 24, 39, 44};
int SIEBEN[6] = {2, 20, 23, 40, 43, 60};
int ACHT[4] = {79, 84, 99, 105};
int NEUN[4] = {41, 42, 61, 62};
int ZEHN[4] = {1, 21, 22, 41};
int ELF[3] = {56, 67, 76};
int ZWOELF[5] = {63, 80, 83, 100, 104};
int WORDS_HOUR[12] = {ZWOELF, EINS, ZWEI, DREI, VIER, FUENF, SECHS, SIEBEN, ACHT, NEUN, ZEHN, ELF};
int SIZEOF_WORDS_HOUR[12] = {5, 4, 4, 4, 4, 4, 5, 6, 4, 4, 4, 3};

int PLUS_MIN[4] = {11, 113, 102, 0};

void LED_setBrightness() {
  FastLED.setBrightness(LED_BRIGHTNESS);
  LED_showStrip();
}

// init routine
void LED_initRoutine() {
  LED_Blackout();
  LED_CURRENT_COL.setColorCode(LED_COL_METEOR);
    meteorRain(15, 64, true, 55);
  //  matrixScreen(1);
}

// show the strip
void LED_showStrip() {
  //  Serial.println("");
  //  Serial.println("SHOW STRIP");
  FastLED.show();
}

// set the the single pixel
void LED_setPixel(int Pixel) {
  // FastLED
  LEDs[Pixel].r = LED_CURRENT_COL.r;
  LEDs[Pixel].g = LED_CURRENT_COL.g;
  LEDs[Pixel].b = LED_CURRENT_COL.b;
}

void LED_blinkALL(size_t blinktime) {

  //  Serial.println("LED fade in ALL start");
  for (size_t blk = 0; blk < blinktime; blk++) {
    LED_CURRENT_COL.setColorCode(LED_COL_CONFIG);
    for (size_t i = 0; i < NUM_LEDS; i++) {
      LED_setPixel(i);
    }
    LED_showStrip();
    //    Serial.println("BLINK");
    delay(500);
    LED_Blackout();
    delay(500);

  }
  //  LED_Blackout();
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
}

// all leds blackout
void LED_Blackout() {
  LED_CURRENT_COL.setColorCode(LED_COL_BLACK);
  for (uint16_t i = 0; i < NUM_LEDS; i++ ) {
    LED_setPixel(i);
  }
  LED_showStrip();
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
  ESIST_OFF = true;
  LAST_HOUR = -1;
  LAST_MIN = -1;
  ALL_OUT = true;
}


void LED_fadeIn(int text[], int size_of) {
  //  Serial.println("LED fade in start");
//  uint8_t fade = 100;
  for (size_t j = 0; j < FADE_VAL; j++) {
    for (uint16_t i = 0; i < size_of; i++ ) {
      LEDs[text[i]].fadeIn(LED_CURRENT_COL, FADE_VAL);
    }
    LED_showStrip();
  }
  //  Serial.println("LED fade in end");
}

void LED_fadeOut(int text[], int size_of) {
  //  Serial.println("LED fade out start");
//  uint8_t fade = 100;
  //  Serial.println(LEDs[text[1]].r);
  for (size_t j = 0; j < FADE_VAL; j++) {

    for (uint16_t i = 0; i < size_of; i++ ) {
      LEDs[text[i]].fadeOut(FADE_VAL);
    }
    //    Serial.println(LEDs[text[1]].r);
    LED_showStrip();
  }
  //  Serial.println(LEDs[text[1]].r);
  //  Serial.println("LED fade out end");
}

void LED_setAM_PM() {
  if (AM_PM_ON || ( C_STATUS != RUNNING)) {
    //    Serial.println("Show AM or PM");
    if (NOW_HOUR >= 12) {
      if (PM_OFF) {
        LED_fadeOut(AM, sizeof(AM) / sizeof(int));
        LED_fadeIn(PM, sizeof(PM) / sizeof(int));
        PM_OFF = false;
        AM_OFF = true;
      }
    } else {
      if (AM_OFF) {
        LED_fadeOut(PM, sizeof(PM) / sizeof(int));
        LED_fadeIn(AM, sizeof(AM) / sizeof(int));
        PM_OFF = true;
        AM_OFF = false;
      }
    }
  }
}
void LED_setHour(int setHour = LAST_HOUR) {
  //  Serial.println("LED Set Hour");
  LED_setAM_PM();
  int lHour = setHour - 1;
  if (lHour == -1) {
    lHour = 11;
  }
  LED_fadeOut(WORDS_HOUR[lHour], SIZEOF_WORDS_HOUR[lHour]);
  LED_fadeIn(WORDS_HOUR[setHour], SIZEOF_WORDS_HOUR[setHour]);
}

void LED_setMinute(int mod_minute) {

  //  Serial.println("LED Set minute");
  if (mod_minute != 0) {
    for (uint8_t i = 0; i < mod_minute; i++) {
      LED_setPixel(PLUS_MIN[i]);
      LED_showStrip();
    }
  } else {
    LED_fadeOut(PLUS_MIN, sizeof(PLUS_MIN) / sizeof(int));
  }

  if (ALL_OUT || (mod_minute == 0)) {
    switch ((NOW_MIN - mod_minute)) {
      case 0:
        LED_fadeOut(FUENF_VOR, sizeof(FUENF_VOR) / sizeof(int));
        LED_fadeIn(UHR, sizeof(UHR) / sizeof(int));
        break;
      case 5:
        LED_fadeOut(UHR, sizeof(UHR) / sizeof(int));
        LED_fadeIn(FUENF_NACH, sizeof(FUENF_NACH) / sizeof(int));
        break;
      case 10:
        LED_fadeOut(FUENF_NACH, sizeof(FUENF_NACH) / sizeof(int));
        LED_fadeIn(ZEHN_NACH, sizeof(ZEHN_NACH) / sizeof(int));
        break;
      case 15:
        LED_fadeOut(ZEHN_NACH, sizeof(ZEHN_NACH) / sizeof(int));
        LED_fadeIn(VIERTEL_NACH, sizeof(VIERTEL_NACH) / sizeof(int));
        break;
      case 20:
        LED_fadeOut(VIERTEL_NACH, sizeof(VIERTEL_NACH) / sizeof(int));
        LED_fadeIn(ZWANZIG_NACH, sizeof(ZWANZIG_NACH) / sizeof(int));
        break;
      case 25:
        LED_fadeOut(ZWANZIG_NACH, sizeof(ZWANZIG_NACH) / sizeof(int));
        LED_fadeIn(FUENF_VOR_HALB, sizeof(FUENF_VOR_HALB) / sizeof(int));
        break;
      case 30:
        LED_fadeOut(FUENF_VOR_HALB, sizeof(FUENF_VOR_HALB) / sizeof(int));
        LED_fadeIn(HALB, sizeof(HALB) / sizeof(int)); break;
      case 35:
        LED_fadeOut(HALB, sizeof(HALB) / sizeof(int));
        LED_fadeIn(FUENF_NACH_HALB, sizeof(FUENF_NACH_HALB) / sizeof(int));
        break;
      case 40:
        LED_fadeOut(FUENF_NACH_HALB, sizeof(FUENF_NACH_HALB) / sizeof(int));
        LED_fadeIn(ZWANZIG_VOR, sizeof(ZWANZIG_VOR) / sizeof(int));
        break;
      case 45:
        LED_fadeOut(ZWANZIG_VOR, sizeof(ZWANZIG_VOR) / sizeof(int));
        LED_fadeIn(VIERTEL_VOR, sizeof(VIERTEL_VOR) / sizeof(int));
        break;
      case 50:
        LED_fadeOut(VIERTEL_VOR, sizeof(VIERTEL_VOR) / sizeof(int));
        LED_fadeIn(ZEHN_VOR, sizeof(ZEHN_VOR) / sizeof(int));
        break;
      case 55:
        LED_fadeOut(ZEHN_VOR, sizeof(ZEHN_VOR) / sizeof(int));
        LED_fadeIn(FUENF_VOR, sizeof(FUENF_VOR) / sizeof(int));
        break;
    }
    ALL_OUT = false;
  }

}




void LED_showESIST() {
  if (ESIST_OFF) {
    //    Serial.println("Show -Es ist-");
    LED_fadeIn(ESIST, sizeof(ESIST) / sizeof(int));
    ESIST_OFF = false;
  }
}

/// --------------------------------------------------------------------------------------------------------------------------------------


void meteorRain(byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {

  for (int i = 0; i < NUM_LEDS + NUM_LEDS; i++) {
    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        //        fadeToBlack(j, meteorTrailDecay );
        LEDs[j].fadeToBlackBy( meteorTrailDecay );
      }
    }
    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
        LED_setPixel(i - j);
      }
    }
    FastLED.show();
    delay(SpeedDelay);
  }

}

void matrixScreen(int eyeSize) {
  for (size_t cyc = 0; cyc < 10; cyc++) {
    for (size_t i = 0; i < 10; i++) {
      LED_Blackout();
      for (int col = 0; col < 11; col++) {
        LED_setPixel(COLUMN[col][i]);
        for (int j = 1; j <= eyeSize; j++) {
          LED_setPixel(COLUMN[col][j + i]);
        }

        LED_setPixel(COLUMN[col][i + eyeSize + 1]);
      }
      LED_showStrip();
      delay(100);
    }
  }
}

void LED_blinkFUNK(int cycletime) {
  Serial.println("Show Funk Blink");
  LED_CURRENT_COL.setColorCode(LED_COL_CONFIG);
  LED_fadeIn(FUNK, sizeof(FUNK) / sizeof(int));
  delay(cycletime);
  LED_fadeOut(FUNK, sizeof(FUNK) / sizeof(int));
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR); 
}
