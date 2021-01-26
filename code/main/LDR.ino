#include <Wire.h>

#define LDR_PIN 10
#define UPDATE_CYC 20
#define LDR_MAX 850
#define LDR_MIN 450

uint16_t val_pre = 0;
uint16_t val = 0;
uint16_t target_bright = 255;
uint16_t val_max = 0;
uint16_t val_min = 255;
uint8_t toleranze = 5;
uint8_t counter = 0;
int16_t delta = 0;

void LDR_init() {
//  pinMode(LDR_PIN, INPUT_PULLUP);
}


void LDR_read() {
  // put your main code here, to run repeatedly:
  if (counter >= UPDATE_CYC) {
    val = analogRead(LDR_PIN);
    if (val <= LDR_MIN) {
      val = LDR_MIN;
    }
    if (val >= LDR_MAX) {
      val = LDR_MAX;
    }
    //    Serial.print("millisec:\t");
    //  Serial.println(millis());
//    Serial.print("Current val:\t");
//    Serial.println(val);
//    Serial.print("Pre val:\t");
//    Serial.println(val_pre);
    val_max = val_pre + toleranze;
    val_min = val_pre - toleranze;
//    Serial.print("Current val:\t");
//    Serial.print(val_max);
//    Serial.print(" - ");
//    Serial.println(val_min);
    if ((val > val_min) & (val < val_max)) {
      val = val_pre;
    }
    else {
//      Serial.print("Set: ");
//      Serial.println(map(val, LDR_MIN, LDR_MAX, 5, 255));
      target_bright = map(val, LDR_MIN, LDR_MAX, 10, 255);
      delta = target_bright - LED_BRIGHTNESS;
      delta = delta/abs(delta);
//      LED_BRIGHTNESS = map(val, LDR_MIN, LDR_MAX, 5, 255);
//      LED_setBrightness();
    }
    val_pre = val;
    counter = 0;
  }
  ++counter;

  LDR_next();
}

void LDR_next(){
//  Serial.print("Target:\t");
//  Serial.println(target_bright);
//  Serial.print("LED_BRIGHTNESS:\t");
//  Serial.println(LED_BRIGHTNESS);
  
  if(target_bright != LED_BRIGHTNESS){
//      Serial.print("Adapt Brightness from:\t");
//      Serial.println(LED_BRIGHTNESS);
//      Serial.print("\tto:\t");
//      Serial.println(target_bright);
  LED_BRIGHTNESS += delta;
      LED_setBrightness();
  }
  }
