#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>
#include <SoftwareSerial.h>
#include "lcd_functions.h"
#include "button_reader.h"
#include <EEPROM.h>
SoftwareSerial LCD = SoftwareSerial(0, txPin);
int noOfAlarms=1;
int alarmHour[3]={1};
int alarmMin[3]={30};
int alarmAM_PM[3] ={0}; // 0 is AM, 1 is PM
long alarmDuartion = 30; // in minutes
long currentTimeHour=1;
long currentTimeMin=30;
bool currentTimeAM_PM=0; // 0 is AM, 1 is PM
bool toUpdateAlarm = false;
bool toUpdateTime = false;
DS3231 ClockX;
void initLCD(){
  LCD.begin(9600);
  backLightOff();
  LCD.write(byte(25));
}

void setAlarm(){
  clearLCD();
  LCD.println("Pumps Per Day");
  newLine();
  LCD.println(noOfAlarms);
  delay(10);
  while(1){ //get number of alarms
    delay(50);
	LCD.write(byte(148)); //move to (1,0)
    int r = readButtons();
    if(r == UP_PIN){
      if(noOfAlarms < 3){
        LCD.write(byte(148)); //move to (1,0)
        LCD.println(++noOfAlarms);
        newLine();
      }
    }else if(r == DOWN_PIN){
      if(noOfAlarms > 1){
        LCD.write(byte(148)); //move to (1,0)
        LCD.println(--noOfAlarms);
        newLine();
      }
    }else if(r == SET_PIN){
      clearLCD();
      LCD.println("Set Alarm Time");
      newLine();
      LCD.println("01:30 AM");
      break;
    }
  }
  
  
  for(int ith_alarm=0;ith_alarm<noOfAlarms;ith_alarm++){ // get timings for all the alarms
  clearLCD();
  LCD.println("Set Alarm No. ");
  LCD.println(ith_alarm+1);
  newLine();
    LCD.println("01:30 AM");
    while(1){ //get hour
      delay(50);
	  LCD.write(byte(148)); //move to (1,0)
      int r = readButtons();
      if(r == UP_PIN && (alarmHour[ith_alarm] < 12)){
          if(alarmHour[ith_alarm] <9){
              	LCD.write(byte(148));
              	delay(10);
              	LCD.println("0");
              	delay(10);
              	LCD.write(byte(149)); //move to (1,1)
				LCD.println(++alarmHour[ith_alarm]);
				LCD.write(byte(149)); //move to (1,1)
          }else{
              	LCD.write(byte(148)); //move to (1,0)
				LCD.println(++alarmHour[ith_alarm]);
				LCD.write(byte(148)); //move to (1,0)
          }
      }else if(r == DOWN_PIN && (alarmHour[ith_alarm] > 0)){
          if(alarmHour[ith_alarm] <= 10){
              	LCD.write(byte(148));
              	delay(10);
              	LCD.println("0");
              	delay(10);
              	LCD.write(byte(149)); //move to (1,1)
				LCD.println(--alarmHour[ith_alarm]);
				LCD.write(byte(149)); //move to (1,1)
          }else{
              	LCD.write(byte(148)); //move to (1,0)
				LCD.println(--alarmHour[ith_alarm]);
				LCD.write(byte(148)); //move to (1,0)
          }
      }else if(r == SET_PIN){
          break;
      }
    }
    
    while(1){ //get minute
      delay(50);
	  LCD.write(byte(151));
      int r = readButtons();
      if(r == UP_PIN && (alarmMin[ith_alarm] < 60)){
          if(alarmMin[ith_alarm] <9){
           	LCD.write(byte(151));
          	delay(10);
          	LCD.println("0");
          	LCD.write(byte(152)); //move to (1,7)
			LCD.println(++alarmMin[ith_alarm]);
			LCD.write(byte(152)); //move to (1,7)
          }else{
      		LCD.write(byte(151)); //move to (1,6)
			LCD.println(++alarmMin[ith_alarm]);
		  	LCD.write(byte(151));
      	  }
      }else if(r == DOWN_PIN && (alarmMin[ith_alarm] > 0)){
          if(alarmMin[ith_alarm] <= 10){
      		LCD.write(byte(151));
      		delay(10);
      		LCD.println("0");
      		LCD.write(byte(152)); //move to (1,7)
			LCD.println(--alarmMin[ith_alarm]);
			LCD.write(byte(152)); //move to (1,7)
      	  }else{
      		LCD.write(byte(151)); //move to (1,6)
			LCD.println(--alarmMin[ith_alarm]);
			LCD.write(byte(151)); //move to (1,6)
      	  }
      }else if(r == SET_PIN){
        break;
      }
    }
  	while(1){ //get AM/PM
      delay(50);
	  LCD.write(byte(154)); //move to (1,3)
      int r = readButtons();
      if(r == UP_PIN && (alarmAM_PM[ith_alarm] == 0)){
          alarmAM_PM[ith_alarm] =1;
          LCD.write(byte(154)); //move to (1,3)
          LCD.println("P");
		  LCD.write(byte(154)); //move to (1,3)
      }else if(r == DOWN_PIN && (alarmAM_PM[ith_alarm] == 1)){
          alarmAM_PM[ith_alarm] = 0;
          LCD.write(byte(154)); //move to (1,3)
          LCD.println("A");
		  LCD.write(byte(154)); //move to (1,3)
      }else if(r == SET_PIN){
        break;
      }
    }
  }
  /*get alarm duration*/
  clearLCD();
  LCD.println("Max Pumping Time");
  newLine();
  LCD.println(alarmDuartion);
  LCD.write(byte(151));
  LCD.println("Minutes");
  while(1){ //get duration
    delay(50);
	LCD.write(byte(148));
    int r = readButtons();
    if(r == UP_PIN && (alarmDuartion < 99)){
    if(alarmDuartion <9){
      LCD.write(byte(148));
      delay(10);
      LCD.println("0");
      delay(10);
      LCD.write(byte(149)); //move to (1,1)
    }else{
      LCD.write(byte(148)); //move to (1,0)
    }
        LCD.println(++alarmDuartion);
		newLine();
    }else if(r == DOWN_PIN && (alarmDuartion > 0)){
    if(alarmDuartion <= 10){
      LCD.write(byte(148));
      delay(10);
      LCD.println("0");
      delay(10);
      LCD.write(byte(149)); //move to (1,1)
    }else{
      LCD.write(byte(148)); //move to (1,0)
    }
    LCD.println(--alarmDuartion);
	newLine();
    }else if(r == SET_PIN){
          break;
    }
  }
  toUpdateAlarm = true;
 
}

void setCurrentTime(){
  clearLCD();
  LCD.println("Set Clock Time");
  delay(10);
  newLine();
  LCD.println("01:30 AM");
   while(1){ //get hour
        delay(50);
		LCD.write(byte(148)); //move to (1,0)
        int r = readButtons();
        if(r == UP_PIN && (currentTimeHour < 12)){
      		if(currentTimeHour <9){
      			LCD.write(byte(148)); //move to (1,0)
     	 		LCD.println("0");
      			LCD.write(byte(149)); //move to (1,1)
				LCD.println(++currentTimeHour);
				LCD.write(byte(149)); //move to (1,1)
      		}else{
      			LCD.write(byte(148)); //move to (1,0)
				LCD.println(++currentTimeHour);
				LCD.write(byte(148)); //move to (1,0)
      		}
          	
        }else if(r == DOWN_PIN && (currentTimeHour > 0)){
      		if(currentTimeHour <= 10){
      			LCD.write(byte(148)); //move to (1,0)
      			LCD.println("0");
      			LCD.write(byte(149)); //move to (1,1)
				LCD.println(--currentTimeHour);
				LCD.write(byte(149)); //move to (1,1)
      		}else{
      			LCD.write(byte(148)); //move to (1,0)
				LCD.println(--currentTimeHour);
				LCD.write(byte(148)); //move to (1,0)
      		}
          	
        }else if(r == SET_PIN){
          break;
        }
   }
   
    while(1){ //get minute
      delay(50);
	  LCD.write(byte(151)); //move to (1,6)
      int r = readButtons();
      if(r == UP_PIN && (currentTimeMin < 60)){
      		if(currentTimeMin <9){
      			LCD.write(byte(151)); //move to (1,6)
      			LCD.println("0");
      			LCD.write(byte(152)); //move to (1,7)
				LCD.println(++currentTimeMin);
				LCD.write(byte(152)); //move to (1,7)
      		}else{
      			LCD.write(byte(151)); //move to (1,6)
				LCD.println(++currentTimeMin);
				LCD.write(byte(151)); //move to (1,6)
      		}
          	
      }else if(r == DOWN_PIN && (currentTimeMin > 0)){
          if(currentTimeMin <=10){
      			LCD.write(byte(151)); //move to (1,6)
      			LCD.println("0");
      			LCD.write(byte(152)); //move to (1,7)
				LCD.println(--currentTimeMin);
				LCD.write(byte(152)); //move to (1,7)
      	  }else{
      			LCD.write(byte(151)); //move to (1,6)
				LCD.println(--currentTimeMin);
				LCD.write(byte(151)); //move to (1,6)
      	  }
         
      }else if(r == SET_PIN){
        break;
      }
    }
  
  while(1){ //get AM/PM
      delay(50);
	  LCD.write(byte(154)); //move to (1,3)
      int r = readButtons();
      if(r == UP_PIN && (currentTimeAM_PM == 0)){
          currentTimeAM_PM =1;
          LCD.write(byte(154)); //move to (1,3)
          LCD.println("P");
		  LCD.write(byte(154)); //move to (1,3)
      }else if(r == DOWN_PIN && (currentTimeAM_PM == 1)){
          currentTimeAM_PM = 0;
          LCD.write(byte(154)); //move to (1,3)
          LCD.println("A");
		  LCD.write(byte(154)); //move to (1,3)
      }else if(r == SET_PIN){
        break;
      }
    }
  toUpdateTime = true; 
}

void EEPROMWritelong(int address, long value){
      //Decomposition from a long to 4 bytes by using bitshift.
      //One = Most significant -> Four = Least significant byte
      byte four = (value & 0xFF);
      byte three = ((value >> 8) & 0xFF);
      byte two = ((value >> 16) & 0xFF);
      byte one = ((value >> 24) & 0xFF);

      //Write the 4 bytes into the eeprom memory.
      EEPROM.write(address, four);
      EEPROM.write(address + 1, three);
      EEPROM.write(address + 2, two);
      EEPROM.write(address + 3, one);
}

void runningState(){
        long alarmTimeInSec=-1;
  if(toUpdateAlarm){
    //write to eeprom
    if(noOfAlarms == 1){
                        //Serial.println("Setting alarm 1");
                        alarmTimeInSec = (((long(alarmHour[0]) * 60) * 60) + (long(alarmMin[0]) * 60)) + (long(alarmAM_PM[0]) * 43200);
                        //Serial.print("alarmTimeInSec >> ");
                        //Serial.println(alarmTimeInSec);
      EEPROMWritelong(100,alarmTimeInSec);
      EEPROMWritelong(105,-1);
      EEPROMWritelong(110,-1);
    }else if(noOfAlarms == 2){
                        //Serial.println("Setting alarm 2");
                        long alarmTimeInSec1 = (((long(alarmHour[0]) * 60) * 60) + (long(alarmMin[0]) * 60)) + (long(alarmAM_PM[0]) * 43200);
                        long alarmTimeInSec2 = (((long(alarmHour[1]) * 60) * 60) + (long(alarmMin[1]) * 60)) + (long(alarmAM_PM[1]) * 43200);
                        long alarmTimeAndDur = alarmTimeInSec1 + (alarmDuartion * 60);
                        //Serial.print("alarmTimeInSec1 >> ");
                        //Serial.println(alarmTimeInSec1);
                        //Serial.print("alarmTimeInSec2 >> ");
                        //Serial.println(alarmTimeInSec2);
                        EEPROMWritelong(100,alarmTimeInSec1);
      EEPROMWritelong(105,alarmTimeInSec2);
                        EEPROMWritelong(110,-1);
    }else if(noOfAlarms == 3){
                        //Serial.println("Setting alarm 3");
                        long alarmTimeInSec1 = (((long(alarmHour[0]) * 60) * 60) + (long(alarmMin[0]) * 60)) + (long(alarmAM_PM[0]) * 43200);
                        long alarmTimeInSec2 = (((long(alarmHour[1]) * 60) * 60) + (long(alarmMin[1]) * 60)) + (long(alarmAM_PM[1]) * 43200);
                        long alarmTimeInSec3 = (((long(alarmHour[2]) * 60) * 60) + (long(alarmMin[2]) * 60)) + (long(alarmAM_PM[2]) * 43200);
                        EEPROMWritelong(100,alarmTimeInSec1);
      EEPROMWritelong(105,alarmTimeInSec2);
      EEPROMWritelong(110,alarmTimeInSec3);
    }
                alarmTimeInSec = alarmDuartion*60;
                //Serial.print("alarmDuartion >> ");
                //Serial.println(alarmTimeInSec);
          EEPROMWritelong(120,alarmTimeInSec);
                alarmHour[3]={1};
                alarmMin[3]={30};
                alarmAM_PM[3] ={0}; // 0 is AM, 1 is PM
    toUpdateAlarm=false;
  }
  if(toUpdateTime){
      //update RTC time here
           long setTimeHour = currentTimeHour;
           if(currentTimeAM_PM == 1){
             setTimeHour = setTimeHour + 12;
           }
           ClockX.setHour(byte(setTimeHour));
           ClockX.setMinute(byte(currentTimeMin));
           ClockX.setSecond(byte(0));
           currentTimeHour=1;
           currentTimeMin=30;
           currentTimeAM_PM=0;
           //dTSet.setTime(setTimeHour,currentTimeMin,0,1,1,2015);
           //RTCX.adjust(dTSet);
     toUpdateTime=false;
  }
  clearLCD();
  backLightOff();
  LCD.println("Chill state!");
  delay(10);
}

void clearLCD(){
  LCD.write(byte(12));
  delay(10);
}
void newLine(){
  LCD.write(byte(13));
  delay(10);
}

void settingsLoop(int item){
  clearLCD();
  LCD.println("Settings");
  delay(10);
  newLine();
  if(item == 1){
    LCD.println("1.Set Pump  Time");
  }else if(item == 2){
    LCD.println("2.Set Clock Time");
  }
}

void backLightOn(){
  LCD.write(byte(17)); //backlight on
}
void backLightOff(){
  LCD.write(byte(18)); //backlight off
}

