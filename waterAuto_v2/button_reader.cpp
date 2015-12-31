#include <Arduino.h>
#include "button_reader.h"
int leftVal,rightVal,upVal,downVal,setVal;
/*Reads all the buttons. Returns integer - which button is active accoridng to the #defs.
Returns -1 is no button is pressed.
*/
int readButtons(){
  int ret=-1;
   //Serial.println("Inside button reader");
  if(digitalRead(LEFT_PIN) == HIGH){
    ret=LEFT_PIN;
    Serial.println("LEFT");
    delay(300);
  }
  if(digitalRead(RIGHT_PIN) == HIGH){
    ret=RIGHT_PIN;
    Serial.println("RIGHT");
    delay(300);
  }
  if(digitalRead(UP_PIN) == HIGH){
    ret=UP_PIN;
    Serial.println("UP");
    delay(300);
  }
  if(digitalRead(DOWN_PIN) == HIGH){
    ret=DOWN_PIN;
    Serial.println("DOWN");
    delay(300);
  }
  if(digitalRead(SET_PIN) == HIGH){
    ret=SET_PIN;
    Serial.println("SET");
    delay(300);
  }
  return ret;
}
