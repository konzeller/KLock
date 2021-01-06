#include <Wire.h>

#define BTN_PIN_1 8

void CS_init(){
  C_STATUS = RUNNING;
  pinMode(BTN_PIN_1, INPUT_PULLUP);
//  digitalWrite(LED_PIN, LOW);
}


// check if and which button is klicked
void CS_checkButtons(){
  int val = 0;
  val = digitalRead(BTN_PIN_1);
  switch(C_STATUS){
    case RUNNING:  if(val == LOW){C_STATUS = CLOCK_ADAPT;delay(100);break;}
    case CLOCK_ADAPT: if(val == LOW){C_STATUS = RUNNING;delay(100);break;} 
  }
  Serial.println(val);
  
}
