/* -----------------------------------------------------------------
   TI-30 Calculator Keypad Simulator
   https://github.com/Andy4495/TI-30-Keypad-Interface

   08-Nov-2023 - Andy4495 - Original

   Sketch to simulate the keypad of a TI-30 calculator

*/

#include "TI30.h"

//     TI30(byte r1, byte r2, byte r3, byte r4, byte r5, byte r6, byte r7,
//     byte c1, byte c2, byte c3, byte c4, byte c5);
TI30 ti30(15, 19, 11, 12, 14, 13, 18,
          10, 9, 8, 7, 6);

KEYNAME theKeys[] = {
/*  SEVEN, FIVE, ZERO, ADD,
  DUMMYKEY, THREE, EQUAL, 
  OnC, PIE, X2, COS */
  OnC,
  ONE, WAIT1000, TWO, WAIT1000, THREE, WAIT1000, ADD,
  FOUR, WAIT500, FIVE, WAIT500, SIX, WAIT500, EQUAL,
  WAIT1000, WAIT1000,
  REPEAT_KEYS
};

keylist myKeys(sizeof(theKeys)/sizeof(theKeys[0]));

#define KEYBOUNCE_CYCLES 1
#define MAX_ROW 8

unsigned long prev_millis = 0;
unsigned long curr_millis;
unsigned long wait_time = 0;
bool scan_trigger = false;
int bounce_count = 0;
KEYNAME current_key = NOKEY;
uint8_t current_row = 1;

// States
enum STATE {
  START_SCAN_CYCLE,
  SCAN_ROWS_1_TO_7,
  START_KEYPRESS,
  CONTINUE_KEYPRESS,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROWS_1_TO_7,
  SKIP_THIS_ROW,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8_PART_2,
  WAIT_STATE
};

STATE state = START_SCAN_CYCLE;

void setup() {
  Serial.begin(9600);
  Serial.println("TI-30 Calculator Keyboard v5");

  ti30.begin();
  for (unsigned int i = 0; i < sizeof(theKeys)/sizeof(theKeys[0]); i++) {
    if (myKeys.add_key(theKeys[i])) {
      Serial.print("Error adding key: ");
      Serial.print(theKeys[i]);
      Serial.print(" index: ");
      Serial.println(i);
    } else {
      Serial.print("Adding key: ");
      Serial.println(theKeys[i]);
    }
  }
}

void loop() {
  curr_millis = millis();

  switch (state) {
    case START_SCAN_CYCLE:
      current_key = myKeys.get_key();
      if (current_key == REPEAT_KEYS) {
        myKeys.start_list();
        current_key = myKeys.get_key();
      }
      if (current_key == END_KEY) {
        myKeys.clear_list();
        current_key = myKeys.get_key();
      }
      if (current_key > WAIT_DELAYS) {
        state = WAIT_STATE;
        prev_millis = curr_millis;
        switch (current_key) {
          case WAIT1000:
            wait_time = 1000;
            break;

          case WAIT500:
            wait_time = 500;
            break;
          
          case WAIT100:
            wait_time = 100;
            break;

          default:
            wait_time = 1000;
            break;
        }
      } else {
        state = SCAN_ROWS_1_TO_7;
        current_row = 1;
        if (current_key != NOKEY) { /// 
          Serial.print("New key: ");         ///
          Serial.println(int(current_key));  ///
        } ///
      }
      break;

    // Check the keypress queue to see if next key is for this row
    case SCAN_ROWS_1_TO_7:
      if (ti30.rowState(current_row) == HIGH) {
        if (current_key >= SPECIALKEYS)  {
          state = SKIP_THIS_ROW;
        } else {
          if ( (ti30.getRow(current_key) + 1) != current_row ) {
            state = SKIP_THIS_ROW;
          } else {
            state = START_KEYPRESS;
          } 
        }
      }
      break;

    // Activate the column corresponding to the key
    case START_KEYPRESS:
      ti30.pressKey(current_key);
      state = CONTINUE_KEYPRESS;
      break;

    // Row scan is active
    case CONTINUE_KEYPRESS:
      if ((current_row < MAX_ROW && ti30.rowState(current_row) == LOW) || 
          (current_row == MAX_ROW && ti30.rowState(1) == HIGH)) {
        ti30.releaseKey(current_key);
        bounce_count++;
        if (bounce_count > KEYBOUNCE_CYCLES) {
          bounce_count = 0;
          state = START_SCAN_CYCLE;
        } else {
          if (current_row < MAX_ROW) state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROWS_1_TO_7;
          else state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8;
        }
      }
      break;

    // Row scan ended, but still want to keep key pressed long enough to
    // get through debounce filter
    case ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROWS_1_TO_7:
      if (current_row < MAX_ROW && ti30.rowState(current_row) == HIGH){
        state = CONTINUE_KEYPRESS;
        ti30.pressKey(current_key);
      }
      break;

    case ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8:
      if (current_row == MAX_ROW && ti30.rowState(current_row - 1) == HIGH) {
        state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8_PART_2;
      }
      break;

    case ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8_PART_2: 
      if (ti30.rowState(current_row - 1) == LOW) {
        state = CONTINUE_KEYPRESS;
        ti30.pressKey(current_key);
      }
      break;
        
    // No key to press for this row at this time, wait for row to go low
    case SKIP_THIS_ROW:
      if (current_row < MAX_ROW) {
        if (ti30.rowState(current_row) == LOW) {
          current_row++;
          if (current_row < MAX_ROW) state = SCAN_ROWS_1_TO_7;  // For row 8, there is no row signal
        }
      } else {
        // Special handling for row 8: need to look when row 1 starts again
        if (ti30.rowState(1) == HIGH) {
          state = START_SCAN_CYCLE;
        }
      }
      break;

    case WAIT_STATE:
      if (curr_millis - prev_millis > wait_time) {
        state = START_SCAN_CYCLE;
      }
      break;

    default:
      Serial.println("ERROR: Unexpected state.");
      state = START_SCAN_CYCLE;
      break;
  }
}
