#include <Wire.h>
#include "FastLED.h"

#define LED_PIN 10
#define NUM_LEDS 114

long LED_REG_COL = 0xffffff;
struct rgbcolour{byte red;byte green;byte blue;};
struct rgbcolour LED_REG_COL_RGB;
struct rgbcolour LED_CONFIG_COL_RGB = {.red = 0xff, .green = 0x00, .blue=0x00};
struct rgbcolour LED_BLACK_COL_RGB = {.red = 0x00, .green = 0x00, .blue=0x00};
struct rgbcolour LED_CURRENT_COL;

CRGB LEDs[NUM_LEDS];
bool ESIST_OFF = true;


int FUNK[4] {35, 48, 55, 68}; // FUNK
int UHR[3] = {82, 101, 103};
int AM[2] = {57, 66};
int PM[2] = {45, 58};

int ESIST[5]   = {10, 12, 32, 51, 52};        // static LED will not change

int LED_MINUTES[44] = {72, 91, 92, 112, 9, 13, 30, 33, 50, 53, 70, 73, 90, 93, 111, 8, 14, 29, 34, 49, 54, 69, 74, 89, 94, 110, 49, 54, 69, 74, 89, 94, 110, 7, 15, 28, 75, 88, 95, 109, 6, 16, 27, 36};
int FUENF_Z[4] = {72, 91, 92, 112};   // fünf vor oder nach
int ZEHN_F[4] = {9, 13, 30, 33};      // zehn vor oder nach
int ZWANZIG_F[7] = {50, 53, 70, 73, 90, 93, 111}; //zwanzig vor oder nach
//int DREIVIERTEL_F[11] = {8, 14, 29, 34, 49, 54, 69, 74, 89, 94, 110}; // dreiviertel
int VIERTEL_F[7] = {49, 54, 69, 74, 89, 94, 110}; // viertel
int VOR[3] = {7, 15, 28};  // vor
int NACH[4] = {75, 88, 95, 109};    // nach
int HALB[4] = {6, 16, 27, 36};    // halb

int LED_HOURS[50] = {5, 17, 26, 3, 37, 77, 86, 97, 107, 4, 18, 25, 38, 78, 85, 98, 106, 76, 87, 96, 108, 3, 19, 24, 39, 44, 2, 20, 23, 40, 43, 60, 79, 84, 99, 105, 41, 42, 61, 62, 1, 21, 22, 56, 67, 63, 80, 83, 100, 104};
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
int SIZEOF_WORDS_HOUR[12] = {5,4,4,4,4,4,5,6,4,4,4,3};

int PLUS_MIN[4] = {11, 113, 102, 0};

void LED_calcColour(){
  LED_REG_COL_RGB.red = (byte)(LED_REG_COL >> 16);
  LED_REG_COL_RGB.green = (byte)((LED_REG_COL >> 8) & 0x00ff);
  LED_REG_COL_RGB.blue = (byte)(LED_REG_COL & 0x0000ff);
  Serial.println("COLOURS");
  Serial.println(LED_REG_COL_RGB.red);
  Serial.println(LED_REG_COL_RGB.green);
  Serial.println(LED_REG_COL_RGB.blue);
}

void LED_setBrightness(){
  FastLED.setBrightness(LED_BRIGHTNESS);
}
// init routine
void LED_initRoutine(){
  
//  for(uint16_t i=0; i<NUM_LEDS; i++){
//    LED_setPixel(i, random(0x40,0xff),random(0x40,0xff), random(0x40,0xff));
//    LED_showStrip();
//    delay(50);
//  }
//  delay(1500);
//  for(int i=0; i<NUM_LEDS*2; i++){
//    fadeToBlackBy(LEDs, NUM_LEDS, 10);
//    delay(10);
//    LED_showStrip();
//  }
  meteorRain(0xff,0xff,0x00,15, 64, true, 55);
  LED_calcColour();
}

// show the strip
void LED_showStrip() {
   FastLED.show();
}

// set the the single pixel
void LED_setPixel(int Pixel) {
   // FastLED
   LEDs[Pixel].r = LED_CURRENT_COL.red;
   LEDs[Pixel].g = LED_CURRENT_COL.green;
   LEDs[Pixel].b = LED_CURRENT_COL.blue;
}

void LED_blinkALL(size_t blinktime){
  LED_CURRENT_COL = LED_CONFIG_COL_RGB;
  for(size_t i=0; i<blinktime; i++){
    for(size_t j=0; j<NUM_LEDS; j++){
      LED_setPixel(j);
    }
    LED_showStrip();
    delay(500);
    LED_Blackout();
    delay(500);
  }
}
// all leds blackout
void LED_Blackout(){
  LED_CURRENT_COL = LED_BLACK_COL_RGB;
  for(uint16_t i = 0; i < NUM_LEDS; i++ ) {
    LED_setPixel(i); 
  }
  LED_showStrip();
  LED_CURRENT_COL = LED_REG_COL_RGB;
  ESIST_OFF = true;
  LAST_HOUR = -1;
  LAST_MIN = -1; 
}

// defined word blackout
void LED_Blackout(int text[], int size_of){
  LED_CURRENT_COL = LED_BLACK_COL_RGB;
  for(uint16_t i = 0; i < size_of; i++ ) {
    LED_setPixel(text[i]); 
  }
  LED_showStrip();
  LED_CURRENT_COL = LED_REG_COL_RGB;
}

// defined word set
void LED_setText(int text[], int size_of) {
  for(uint16_t i = 0; i<size_of; i++){
     LED_setPixel(text[i]);
  }
  LED_CURRENT_COL = LED_REG_COL_RGB;
}

// defined pixel on
void LED_setText(int text) {
  LED_setPixel(text);
  LED_CURRENT_COL = LED_REG_COL_RGB;
}

void LED_setAM_PM(){
  if(NOW_HOUR >= 12){
    LED_Blackout(AM, sizeof(AM)/sizeof(int));
    LED_setText(PM, sizeof(PM)/sizeof(int));
  }else{
    LED_Blackout(PM, sizeof(PM)/sizeof(int));
    LED_setText(AM, sizeof(AM)/sizeof(int));
  }
}
void LED_setHour(int setHour = LAST_HOUR){
  LED_setAM_PM();
  LED_Blackout(LED_HOURS, sizeof(LED_HOURS)/sizeof(int));
  LED_setText(WORDS_HOUR[setHour],SIZEOF_WORDS_HOUR[setHour]);
}

void LED_setMinute(int mod_minute){
    LED_Blackout(LED_MINUTES, sizeof(LED_MINUTES)/sizeof(int));
    LED_Blackout(UHR, sizeof(UHR)/sizeof(int));
    LED_Blackout(PLUS_MIN, sizeof(PLUS_MIN)/sizeof(int));
    switch((NOW_MIN-mod_minute)){
      case 0: LED_setText(UHR, sizeof(UHR)/sizeof(int));break;
      case 5: LED_setText(FUENF_Z, sizeof(FUENF_Z)/sizeof(int));LED_setText(NACH, sizeof(NACH)/sizeof(int));break;
      case 10: LED_setText(ZEHN_F, sizeof(ZEHN_F)/sizeof(int));LED_setText(NACH, sizeof(NACH)/sizeof(int));break;
      case 15: LED_setText(VIERTEL_F, sizeof(VIERTEL_F)/sizeof(int));LED_setText(NACH, sizeof(NACH)/sizeof(int));break;
      case 20: LED_setText(ZWANZIG_F, sizeof(ZWANZIG_F)/sizeof(int));LED_setText(NACH, sizeof(NACH)/sizeof(int));break;
      case 25: LED_setText(FUENF_Z, sizeof(FUENF_Z)/sizeof(int));LED_setText(VOR, sizeof(VOR)/sizeof(int));LED_setText(HALB, sizeof(HALB)/sizeof(int));break;
      case 30: LED_setText(HALB, sizeof(HALB)/sizeof(int));break;
      case 35: LED_setText(FUENF_Z, sizeof(FUENF_Z)/sizeof(int));LED_setText(NACH, sizeof(NACH)/sizeof(int));LED_setText(HALB, sizeof(HALB)/sizeof(int));break;
      case 40: LED_setText(ZWANZIG_F, sizeof(ZWANZIG_F)/sizeof(int));LED_setText(VOR, sizeof(VOR)/sizeof(int));break;
      case 45: LED_setText(VIERTEL_F, sizeof(VIERTEL_F)/sizeof(int));LED_setText(VOR, sizeof(VOR)/sizeof(int));break;
      case 50: LED_setText(ZEHN_F, sizeof(ZEHN_F)/sizeof(int));LED_setText(VOR, sizeof(VOR)/sizeof(int));break;
      case 55: LED_setText(FUENF_Z, sizeof(FUENF_Z)/sizeof(int));LED_setText(VOR, sizeof(VOR)/sizeof(int));break;
    }
    if(mod_minute != 0){
    for(uint8_t i = 0; i<mod_minute; i++){
      LED_setText(PLUS_MIN[i]);
    }
  }
}




void LED_showESIST(){
  if(ESIST_OFF){LED_setText(ESIST, sizeof(ESIST)/sizeof(int));ESIST_OFF=false;}
}

/// --------------------------------------------------------------------------------------------------------------------------------------
//void loop() {
//  meteorRain(0xff,0xff,0xff,10, 64, true, 30);
//}

void meteorRain(byte r, byte g, byte b, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {  
  LED_Blackout();
  struct rgbcolour meteorColour = {.red =r, .green = g, .blue=b};
  LED_CURRENT_COL = meteorColour;
  for(int i = 0; i < NUM_LEDS+NUM_LEDS; i++) {
   
   
    // fade brightness all LEDs one step
    for(int j=0; j<NUM_LEDS; j++) {
      if( (!meteorRandomDecay) || (random(10)>5) ) {
//        fadeToBlack(j, meteorTrailDecay );
        LEDs[j].fadeToBlackBy( meteorTrailDecay );       
      }
    }
   
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
      if( ( i-j <NUM_LEDS) && (i-j>=0) ) {
        LED_setPixel(i-j);
      }
    }
   
    LED_showStrip();
    delay(SpeedDelay);
  }
  LED_CURRENT_COL = LED_REG_COL_RGB;
}

void LED_blinkFUNK(int cycletime){
  LED_CURRENT_COL = LED_CONFIG_COL_RGB;
  LED_setText(FUNK, sizeof(FUNK)/sizeof(int));
  LED_showStrip();
  delay(cycletime);
  LED_Blackout();
  LED_showStrip();
}
