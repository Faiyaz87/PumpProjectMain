
typedef enum {
  STATE_RUNNING = 1,
  STATE_SETTINGS_ONE = 2,
  STATE_SETTINGS_TWO = 3,
  STATE_SET_ALARM = 4,
  STATE_SET_TIME = 5,
  STATE_SET_DATE = 6
} state_t;

typedef enum {
  LEFT_PRESSED = 1,
  RIGHT_PRESSED = 2,
  UP_PRESSED = 3,
  DOWN_PRESSED = 4,
  SET_PRESSED = 5,
  NOTHING_PRESSED = 6
} event_t;

/*Our Finite State Machine*/
typedef struct {
  state_t state;              //  Current state
  event_t event;              //  Current event
} state_machine;

state_machine *nextState(state_machine *fsm);