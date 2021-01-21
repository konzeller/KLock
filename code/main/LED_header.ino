#include <Wire.h>
#include "FastLED.h"

#define LED_PIN 21 //10
#define NUM_LEDS 114
#define FADE_VAL 100
#define COL_FADE 0x69

#define LED_COL_METEOR CRGB::OrangeRed
#define LED_COL_CONFIG CRGB::Red

int LED_COL_REGULAR = CRGB::White;
CRGB LED_CURRENT_COL(LED_COL_REGULAR);

CRGB LEDs[NUM_LEDS];

bool ALL_OUT = true;
bool ESIST_OFF = true;
bool AM_OFF = true;
bool PM_OFF = true;

uint8_t COLUMN[11][10] = {{10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
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

uint8_t FUNK[4] = {35, 48, 55, 68}; // FUNK
uint8_t UHR[3] = {82, 101, 103};
uint8_t AM[2] = {57, 66};
uint8_t PM[2] = {45, 58};

uint8_t ESIST[5]   = {10, 12, 32, 51, 52};        // static LED will not change

//int LED_MINUTES[44] = {72, 91, 92, 112, 9, 13, 30, 33, 50, 53, 70, 73, 90, 93, 111, 8, 14, 29, 34, 49, 54, 69, 74, 89, 94, 110, 49, 54, 69, 74, 89, 94, 110, 7, 15, 28, 75, 88, 95, 109, 6, 16, 27, 36};
uint8_t FUENF_NACH[8] = {72, 91, 92, 112, 75, 88, 95, 109};   // fünf nach
uint8_t ZEHN_NACH[8] = {9, 13, 30, 33, 75, 88, 95, 109};      // zehn vor oder nach
uint8_t ZWANZIG_NACH[11] = {50, 53, 70, 73, 90, 93, 111, 75, 88, 95, 109}; //zwanzig vor oder nach

//int DREIVIERTEL_F[11] = {8, 14, 29, 34, 49, 54, 69, 74, 89, 94, 110}; // dreiviertel
uint8_t VIERTEL_NACH[11] = {49, 54, 69, 74, 89, 94, 110, 75, 88, 95, 109}; // viertel
uint8_t FUENF_VOR_HALB[11] = {72, 91, 92, 112, 7, 15, 28, 6, 16, 27, 36};
uint8_t HALB[4] = {6, 16, 27, 36};    // halb
uint8_t FUENF_NACH_HALB[12] = {72, 91, 92, 112, 75, 88, 95, 109, 6, 16, 27, 36};
uint8_t FUENF_VOR[7] = {72, 91, 92, 112, 7, 15, 28};   // fünf nach
uint8_t ZEHN_VOR[7] = {9, 13, 30, 33, 7, 15, 28};      // zehn vor oder nach
uint8_t ZWANZIG_VOR[10] = {50, 53, 70, 73, 90, 93, 111, 7, 15, 28}; //zwanzig vor oder nach
uint8_t VIERTEL_VOR[10] = {49, 54, 69, 74, 89, 94, 110, 7, 15, 28}; // viertel

//uint8_t LED_HOURS[50] = {5, 17, 26, 3, 37, 77, 86, 97, 107, 4, 18, 25, 38, 78, 85, 98, 106, 76, 87, 96, 108, 3, 19, 24, 39, 44, 2, 20, 23, 40, 43, 60, 79, 84, 99, 105, 41, 42, 61, 62, 1, 21, 22, 56, 67, 63, 80, 83, 100, 104};
uint8_t EINS[4] = {5, 17, 26, 37};
uint8_t ZWEI[4] = {77, 86, 97, 107};
uint8_t DREI[4] = {4, 18, 25, 38};
uint8_t VIER[4] = {78, 85, 98, 106};
uint8_t FUENF[4] = {76, 87, 96, 108};
uint8_t SECHS[5] = {3, 19, 24, 39, 44};
uint8_t SIEBEN[6] = {2, 20, 23, 40, 43, 60};
uint8_t ACHT[4] = {79, 84, 99, 105};
uint8_t NEUN[4] = {41, 42, 61, 62};
uint8_t ZEHN[4] = {1, 21, 22, 41};
uint8_t ELF[3] = {56, 67, 76};
uint8_t ZWOELF[5] = {63, 80, 83, 100, 104};
uint16_t WORDS_HOUR[12] = {ZWOELF, EINS, ZWEI, DREI, VIER, FUENF, SECHS, SIEBEN, ACHT, NEUN, ZEHN, ELF};
uint8_t SIZEOF_WORDS_HOUR[12] = {5, 4, 4, 4, 4, 4, 5, 6, 4, 4, 4, 3};

uint8_t PLUS_MIN[4] = {11, 113, 102, 0};

void LED_setBrightness() {
//  Serial.println("SET BRIGHTNESS");
  FastLED.setBrightness(LED_BRIGHTNESS);
  FastLED.show();
}

// init routine
void LED_initRoutine() {
  LED_Blackout();
  LED_CURRENT_COL.setColorCode(LED_COL_METEOR);
  LED_meteorRain(15, 64, true, 55);
}

// show the strip
void LED_showStrip() {
  //  Serial.println("");
  //  Serial.println("SHOW STRIP");
  FastLED.show();
}

// set the the single pixel
void LED_setPixel(uint8_t Pixel) {
  // FastLED
  LEDs[Pixel].r = LED_CURRENT_COL.r;
  LEDs[Pixel].g = LED_CURRENT_COL.g;
  LEDs[Pixel].b = LED_CURRENT_COL.b;
}

void LED_setPixelColor(uint8_t Pixel, byte r, byte g, byte b) {
  // FastLED
  LEDs[Pixel].r = r;
  LEDs[Pixel].g = g;
  LEDs[Pixel].b = b;
}

void LED_blinkALL(uint8_t blinktime) {

  //  Serial.println("LED fade in ALL start");
  for (uint8_t blk = 0; blk < blinktime; ++blk) {
    LED_CURRENT_COL.setColorCode(LED_COL_CONFIG);
    for (uint8_t i = 0; i < NUM_LEDS; ++i) {
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
  LED_CURRENT_COL.setColorCode(CRGB::Black);
  for (uint8_t i = 0; i < NUM_LEDS; ++i) {
    LED_setPixel(i);
  }
  LED_showStrip();
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
  ESIST_OFF = true;
  LAST_HOUR = -1;
  LAST_MIN = -1;
  ALL_OUT = true;
}


void LED_fadeIn(uint8_t text[], uint8_t size_of) {
  //  Serial.println("LED fade in start");
  //  uint8_t fade = 100;
  for (uint8_t j = 0; j < FADE_VAL; ++j) {
    for (uint8_t i = 0; i < size_of; ++i) {
      LEDs[text[i]].fadeIn(LED_CURRENT_COL, FADE_VAL);
    }
    LED_showStrip();
  }
  //  Serial.println("LED fade in end");
}

void LED_fadeOut(uint8_t text[], uint8_t size_of) {
  //  Serial.println("LED fade out start");
  //  uint8_t fade = 100;
  //  Serial.println(LEDs[text[1]].r);
  for (uint8_t j = 0; j < FADE_VAL; ++j) {

    for (uint8_t i = 0; i < size_of; ++i ) {
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
//        Serial.println("Show AM or PM");
    if (NOW_HOUR >= 12) {
      if (PM_OFF) {
        LED_fadeOut(AM, sizeof(AM) / sizeof(uint8_t));
        LED_fadeIn(PM, sizeof(PM) / sizeof(uint8_t));
        PM_OFF = false;
        AM_OFF = true;
      }
    } else {
      if (AM_OFF) {
        LED_fadeOut(PM, sizeof(PM) / sizeof(uint8_t));
        LED_fadeIn(AM, sizeof(AM) / sizeof(uint8_t));
        PM_OFF = true;
        AM_OFF = false;
      }
    }
  }
}
void LED_setHour(int8_t setHour = LAST_HOUR) {
  
  LED_setAM_PM();
  int8_t lHour = setHour - 1;
  if (lHour == -1) {
    lHour = 11;
  }
//  Serial.println("LED Set Hour");
//  Serial.println(setHour);
//Serial.println("SET HOUR");
//    Serial.println(lHour);
//    Serial.println(setHour);
  LED_fadeOut(WORDS_HOUR[lHour], SIZEOF_WORDS_HOUR[lHour]);
  LED_fadeIn(WORDS_HOUR[setHour], SIZEOF_WORDS_HOUR[setHour]);
}

void LED_setMinute(uint8_t mod_minute) {

//  Serial.println("LED Time: ");
//  Serial.print(NOW_HOUR);
//  Serial.print(":");
//  Serial.print(":");
//  Serial.print(NOW_MIN);
//  Serial.print(":");
//  Serial.println(NOW_SEC);
  if (mod_minute != 0) {
    for (uint8_t i = 0; i < mod_minute; ++i) {
      LED_setPixel(PLUS_MIN[i]);
      LED_showStrip();
    }
  } else {
    for (uint8_t i = 0; i < sizeof(PLUS_MIN) / sizeof(uint8_t); ++i) {
      LED_setPixelColor(PLUS_MIN[i], 0x00, 0x00, 0x00);
    }
    LED_showStrip();
  }

  if (ALL_OUT || (mod_minute == 0)) {
    switch ((NOW_MIN - mod_minute)) {
      case 0:
        LED_fadeOut(FUENF_VOR, sizeof(FUENF_VOR) / sizeof(uint8_t));
        
        if((C_STATUS == RUNNING)&(LAST_HOUR!=-1)){LED_matrixScreen();}
        
        LED_fadeIn(UHR, sizeof(UHR) / sizeof(uint8_t));
        break;
      case 5:
        LED_fadeOut(UHR, sizeof(UHR) / sizeof(uint8_t));
        LED_fadeIn(FUENF_NACH, sizeof(FUENF_NACH) / sizeof(uint8_t));
        break;
      case 10:
        LED_fadeOut(FUENF_NACH, sizeof(FUENF_NACH) / sizeof(uint8_t));
        LED_fadeIn(ZEHN_NACH, sizeof(ZEHN_NACH) / sizeof(uint8_t));
        break;
      case 15:
        LED_fadeOut(ZEHN_NACH, sizeof(ZEHN_NACH) / sizeof(uint8_t));
        LED_fadeIn(VIERTEL_NACH, sizeof(VIERTEL_NACH) / sizeof(uint8_t));
        break;
      case 20:
        LED_fadeOut(VIERTEL_NACH, sizeof(VIERTEL_NACH) / sizeof(uint8_t));
        LED_fadeIn(ZWANZIG_NACH, sizeof(ZWANZIG_NACH) / sizeof(uint8_t));
        break;
      case 25:
        LED_fadeOut(ZWANZIG_NACH, sizeof(ZWANZIG_NACH) / sizeof(uint8_t));
        LED_fadeIn(FUENF_VOR_HALB, sizeof(FUENF_VOR_HALB) / sizeof(uint8_t));
        break;
      case 30:
        LED_fadeOut(FUENF_VOR_HALB, sizeof(FUENF_VOR_HALB) / sizeof(uint8_t));
        LED_fadeIn(HALB, sizeof(HALB) / sizeof(uint8_t)); break;
      case 35:
        LED_fadeOut(HALB, sizeof(HALB) / sizeof(uint8_t));
        LED_fadeIn(FUENF_NACH_HALB, sizeof(FUENF_NACH_HALB) / sizeof(uint8_t));
        break;
      case 40:
        LED_fadeOut(FUENF_NACH_HALB, sizeof(FUENF_NACH_HALB) / sizeof(uint8_t));
        LED_fadeIn(ZWANZIG_VOR, sizeof(ZWANZIG_VOR) / sizeof(uint8_t));
        break;
      case 45:
        LED_fadeOut(ZWANZIG_VOR, sizeof(ZWANZIG_VOR) / sizeof(uint8_t));
        LED_fadeIn(VIERTEL_VOR, sizeof(VIERTEL_VOR) / sizeof(uint8_t));
        break;
      case 50:
        LED_fadeOut(VIERTEL_VOR, sizeof(VIERTEL_VOR) / sizeof(uint8_t));
        LED_fadeIn(ZEHN_VOR, sizeof(ZEHN_VOR) / sizeof(uint8_t));
        break;
      case 55:
        LED_fadeOut(ZEHN_VOR, sizeof(ZEHN_VOR) / sizeof(uint8_t));
        LED_fadeIn(FUENF_VOR, sizeof(FUENF_VOR) / sizeof(uint8_t));
        break;
    }
    ALL_OUT = false;
  }

}




void LED_showESIST() {
  if (ESIST_OFF) {
    //    Serial.println("Show -Es ist-");
    LED_fadeIn(ESIST, sizeof(ESIST) / sizeof(uint8_t));
    ESIST_OFF = false;
  }
}

/// --------------------------------------------------------------------------------------------------------------------------------------


void LED_meteorRain(byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, uint8_t SpeedDelay) {

  for (uint8_t i = 0; i < NUM_LEDS + NUM_LEDS; ++i) {
    // fade brightness all LEDs one step
    for (uint8_t j = 0; j < NUM_LEDS; ++j) {
      if ( (!meteorRandomDecay) || (random(10) > 5) ) {
        //        fadeToBlack(j, meteorTrailDecay );
        LEDs[j].fadeToBlackBy( meteorTrailDecay );
      }
    }
    // draw meteor
    for (uint8_t j = 0; j < meteorSize; ++j) {
      if ( ( i - j < NUM_LEDS) && (i - j >= 0) ) {
        LED_setPixel(i - j);
      }
    }
    FastLED.show();
    delay(SpeedDelay);
  }

}

void LED_matrixScreen() {
  int8_t High_rows = 10;
  int8_t Width_col = 11;
  uint8_t SpeedDelay = 160;
  int Pixel = 0xAFFFAF;
  uint8_t TrailDelay = 160;

  for (int8_t times = 0; times < 60; ++times) {

    for (int8_t row = High_rows - 1; row >= 0; --row)
    {
      for (int8_t col = 0; col < Width_col; ++col)
      {
        if (LEDs[COLUMN[col][row]] == CRGB(Pixel))
        {
          LEDs[COLUMN[col][row]] = CRGB(0x006400);//(0, 130, 0); // create trail
          if (row < High_rows - 1) LEDs[COLUMN[col][row + 1]] = CRGB(Pixel);
        }
      }
    }

    // fade all leds
    for (uint8_t i = 0; i < NUM_LEDS; ++i) {
      if (LEDs[i].g != 255) LEDs[i].nscale8(TrailDelay); // only fade trail
    }

    // check for empty screen to ensure code spawn
    bool emptyScreen = true;
    for (uint8_t i = 0; i < NUM_LEDS; ++i) {
      if (LEDs[i])
      {
        emptyScreen = false;
        break;
      }
    }
    if (random8(2) == 0 || emptyScreen) // lower number == more frequent spawns
    {
      int8_t spawnX = random8(Width_col);
      LEDs[COLUMN[spawnX][0]] = CRGB(Pixel);
    }

    FastLED.show();
    delay(SpeedDelay);
  }

  for (uint8_t j = 0; j < NUM_LEDS*2; ++j) {
    for (uint8_t i = 0; i < NUM_LEDS; ++i) {
      LEDs[i].fadeToBlackBy(10);
    }
    FastLED.show();
  }
  for (uint8_t j = 0; j < NUM_LEDS*2; ++j) {
    for (uint8_t i = 0; i < NUM_LEDS; ++i ) {
      LED_setPixelColor(LEDs[i],0x00,0x00,0x00);
    }
    FastLED.show();
  }
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
  ESIST_OFF = true;
  LAST_HOUR = -1;
  ALL_OUT = true;
  LED_setBrightness();
}

void LED_blinkFUNK(uint8_t cycletime) {
  //  Serial.println("Show Funk Blink");
  LED_CURRENT_COL.setColorCode(LED_COL_CONFIG);
  LED_fadeIn(FUNK, sizeof(FUNK) / sizeof(uint8_t));
  delay(cycletime);
  LED_fadeOut(FUNK, sizeof(FUNK) / sizeof(uint8_t));
  delay(FUNKCYCLE);
  LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
}

// TBD -- connection to Button
void LED_changeREG_COL() {

//  Serial.println("Change Color");
//  Serial.println("--");
  if (LED_COL_REGULAR <= 0xFFFFFF) {
    LED_COL_REGULAR = LED_COL_REGULAR + COL_FADE;
    LED_CURRENT_COL.setColorCode(LED_COL_REGULAR);
  } else {
    LED_COL_REGULAR = 0;
  }
//  Serial.println((LED_COL_REGULAR), HEX);
//  Serial.println("--");

}
