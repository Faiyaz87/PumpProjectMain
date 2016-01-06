
#include <Wire.h>
#include <Time.h>
#include <DS3231.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include "state_machine.h"
#include "lcd_functions.h"
#include "button_reader.h"

static int noOfAlarms=0;
//int leftVal,rightVal,upVal,downVal,setVal;
int settingsItems=2; //number of settings

DS3231 Clock;
int aTAddress1 = 100;
int aTAddress2 = 105;
int aTAddress3 = 110;
int aDAddress = 120;
long alaramTime1 = -1;
long alaramTime2 = -1;
long alaramTime3 = -1;
long alaramDuration = -1;
int alarm1Stat;
int alarm2Stat;
int alarm3Stat;
int lastStatus = 0;
boolean isNextDay = false;
long curr_TodayInSecBck = 0;

int motorPin = 3;
int tankWaterPin = A0;
int loopCounter = 0;
int tankfullPin = 6;
int systemOnPin = 5;
int pumpOnPin = 4;

int directWaterPin = A1;
int tankWaterValue =0;
int directWaterValue = 0;
int directWaterDurationCount = 0;

long curr_TodayInSec = 0;
int tankLevelCount = 0;
int motorFunCnt = 0;

bool h12;
bool PM;

state_machine fsm = {STATE_RUNNING,NOTHING_PRESSED};
void setup() {
  Wire.begin();
  pinMode(LEFT_PIN,INPUT);
  pinMode(RIGHT_PIN,INPUT);
  pinMode(UP_PIN,INPUT);
  pinMode(DOWN_PIN,INPUT);
  pinMode(SET_PIN,INPUT);
  Serial.begin(9600);
  initLCD();
  clearLCD();
  runningState();
  pinMode(tankfullPin, OUTPUT);
  pinMode(systemOnPin, OUTPUT);
  pinMode(pumpOnPin, OUTPUT);
  digitalWrite(systemOnPin, HIGH);
}



void loop(){       
   motorFunCnt = motorFunCnt + 1;
   if(motorFunCnt > 24){
     MotorTimerMainLoop();
     motorFunCnt=0;
   }
  SettingAndDiisplay();
  delay(200);
}


void MotorTimerMainLoop(){
    long curr_Hour = long(Clock.getHour(h12, PM));
    long curr_Minute = long(Clock.getMinute());
    long curr_Second = long(Clock.getSecond());
    Serial.println(curr_Hour);
    Serial.println(curr_Minute);
    Serial.println(curr_Second);
    curr_TodayInSec = curr_Hour*3600;
    curr_TodayInSec += curr_Minute*60;
    curr_TodayInSec += curr_Second;
    Serial.print("curr_TodayInSec >> ");
    Serial.println(curr_TodayInSec);
    alaramTime1 = EEPROMReadlong(aTAddress1);
    alaramTime2 = EEPROMReadlong(aTAddress2);
    alaramTime3 = EEPROMReadlong(aTAddress3);
    Serial.print("alaramTime1 >> ");
    Serial.println(alaramTime1);
    Serial.print("alaramTime2 >> ");
    Serial.println(alaramTime2);
    Serial.print("alaramTime3 >> ");
    Serial.println(alaramTime3);
    alaramDuration = EEPROMReadlong(aDAddress);
    Serial.print("alaramDuration >> ");
    Serial.println(alaramDuration);
    tankWaterValue = analogRead(tankWaterPin);
    Serial.println("tank WaterValue");
    Serial.println(tankWaterValue);
    if(tankWaterValue >= 250){
      digitalWrite(tankfullPin, HIGH);
    }else{
      digitalWrite(tankfullPin, LOW);
    }
  
    alarm1Stat = getAlarmStatus(curr_TodayInSec,alaramTime1,alaramDuration,lastStatus);
    alarm2Stat = getAlarmStatus(curr_TodayInSec,alaramTime2,alaramDuration,lastStatus);
    alarm3Stat = getAlarmStatus(curr_TodayInSec,alaramTime3,alaramDuration,lastStatus);
    Serial.println("alarm Stat");
    Serial.println(alarm1Stat);
    Serial.println(alarm2Stat);
    Serial.println(alarm3Stat);
    if(alarm1Stat == 1){
      SwitchOnMotor();
    }else if(alarm2Stat == 1){
      SwitchOnMotor();
    }else if(alarm3Stat == 1){
      SwitchOnMotor();
    }else{
      SwitchOffMotor();
      if (alarm1Stat == 2 || alarm2Stat == 2 || alarm3Stat ==2) {
        delay(180000);
      }
    }
}

int getAlarmStatus(long curr_TodayInSec,long alaramTime,long alaramDuration,int lastStatus){
      //System.out.println("alaramTime >> "+alaramTime);
      if(alaramTime == -1){
        return 0;
      }
      if (curr_TodayInSecBck != 0) {
        if (curr_TodayInSec < curr_TodayInSecBck) {
          isNextDay = true;
        }
      }
      //System.out.println("curr_TodayInSecBck: "+curr_TodayInSecBck+" <><> curr_TodayInSec: "+curr_TodayInSec);
      curr_TodayInSecBck = curr_TodayInSec;
      //System.out.println("curr_TodayInSecBck: "+curr_TodayInSecBck+" <><> curr_TodayInSec: "+curr_TodayInSec);
      //System.out.println("isNextDay :"+isNextDay);
      if (isNextDay) {
        curr_TodayInSec = curr_TodayInSec +  86400;
      }
      //System.out.println("curr_TodayInSec 222 > "+curr_TodayInSec);
      int returnVal = lastStatus;
      long alermDurSum = (alaramTime + alaramDuration);
      //System.out.println("alermDurSum > "+alermDurSum);
      if (curr_TodayInSec >= alaramTime &&  curr_TodayInSec <= alermDurSum) {
        directWaterValue = analogRead(directWaterPin); 
        Serial.println("directWaterValue");
        Serial.println(directWaterValue);
        if(tankWaterValue >=300){
          tankLevelCount = tankLevelCount + 1;
          if(tankLevelCount > 6){
            tankLevelCount=0;
            returnVal = 0;
          }
        }else{
          tankLevelCount=0;
          if(directWaterValue < 300){
            directWaterDurationCount = directWaterDurationCount + 1;
            if(directWaterDurationCount > 6){
              directWaterDurationCount=0;
              returnVal = 2;
              //delay(180000);
            }else{
              returnVal = 1;
            }
          }else{
            directWaterDurationCount = 0;
            returnVal = 1;
          }
        }
      }else{
        isNextDay = false;
        returnVal = 0;
      }
      return returnVal;
}


void SettingAndDiisplay(){
  // put your main code here, to run repeatedly:
  int rc = readButtons();
  if(rc != -1){
    //some button(s) active
    //ToDo -- Switch Case of actions to take...
    switch(rc){
      case LEFT_PIN:
        fsm.event=LEFT_PRESSED;
        break;
      case RIGHT_PIN:
        fsm.event=RIGHT_PRESSED;
        break;
      case UP_PIN:
        fsm.event=UP_PRESSED;
        break;
      case DOWN_PIN:
        fsm.event=DOWN_PRESSED;
        break;
      case SET_PIN:
        fsm.event=SET_PRESSED;
        break;
    }
    //Serial.println("Sending to state_next");
    //Serial.println(rc);
    nextState(&fsm);
  }
}


void SwitchOffMotor(){
  Serial.println("MOTOR OFF called . ");
  lastStatus = 0;
  digitalWrite(motorPin, LOW);
  digitalWrite(pumpOnPin, LOW);
}

void SwitchOnMotor(){
  Serial.println("MOTOR ON called . ");
  lastStatus = 1;
  digitalWrite(motorPin, HIGH);
  digitalWrite(pumpOnPin, HIGH);
}

long EEPROMReadlong(long address){
      //Read the 4 bytes from the eeprom memory.
      long four = EEPROM.read(address);
      long three = EEPROM.read(address + 1);
      long two = EEPROM.read(address + 2);
      long one = EEPROM.read(address + 3);

      //Return the recomposed long by using bitshift.
      return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}