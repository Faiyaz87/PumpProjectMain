#include "state_machine.h"
#include "lcd_functions.h"

state_machine *nextState(state_machine *fsm) {
  if (fsm->state == STATE_RUNNING) {
    if (fsm->event == SET_PRESSED) {
      fsm->state = STATE_SETTINGS_ONE;
          //CALL settings main
	        backLightOn();
          settingsLoop(1);
    }
  } else if (fsm->state == STATE_SETTINGS_ONE) {
    if (fsm->event == SET_PRESSED) {
      fsm->state = STATE_SET_ALARM;
      //CALL Set alarm function
      setAlarm();
    } else if (fsm->event == DOWN_PRESSED) {
      fsm->state = STATE_SETTINGS_TWO;
      //call settings TWO
      settingsLoop(2);
    }else if (fsm->event == LEFT_PRESSED) {
      fsm->state = STATE_RUNNING;
      //call RUNNING STATE
      runningState();
    }
  }else if (fsm->state == STATE_SETTINGS_TWO) {
    if (fsm->event == SET_PRESSED) {
      fsm->state = STATE_SET_TIME;
      //CALL Set time function
      setCurrentTime();
    } else if (fsm->event == UP_PRESSED) {
      fsm->state = STATE_SETTINGS_ONE;
      //call settings ONE
      settingsLoop(1);
    }else if (fsm->event == LEFT_PRESSED) {
      fsm->state = STATE_RUNNING;
      //call RUNNING STATE
      runningState();
    }
  }else if(fsm->state == STATE_SET_ALARM){
	if (fsm->event == LEFT_PRESSED) {
      fsm->state = STATE_SETTINGS_ONE;
      //CALL Set time function
	  settingsLoop(1);
    }else if(fsm->event == SET_PRESSED){
		fsm->state = STATE_RUNNING;
		runningState();
	}
  }else if(fsm->state == STATE_SET_TIME){
	if (fsm->event == LEFT_PRESSED) {
      fsm->state = STATE_SETTINGS_TWO;
      //CALL Set time function
	  settingsLoop(2);
    }else if(fsm->event == SET_PRESSED){
		fsm->state = STATE_RUNNING;
		runningState();
	}
  }
}
