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

keylist myKeys(32);

#define LOOP_DELAY 1000
#define KEYBOUNCE_CYCLES 1
#define MAX_ROW 8

unsigned long prev_millis = 0;
unsigned long curr_millis;
bool scan_trigger = false;
int bounce_count = 0;
KEYNAME current_key = NO_KEY;
uint8_t current_row = 1;

// States
enum STATE {
  START_SCAN_CYCLE,
  NEXT_ROW_SCAN,
  IS_KEY_IN_THIS_ROW,
  START_KEYPRESS,
  CONTINUE_KEYPRESS,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN,
  SKIP_THIS_ROW,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8_PART_2,
  WAIT_A_SECOND  /// Delete this once done
};

STATE state = START_SCAN_CYCLE;

void setup() {
  Serial.begin(9600);
  Serial.println("TI-30 Calculator Keyboard v3");
  ti30.begin();
  if (myKeys.add_key(SEVEN)) Serial.println("Error adding key");
  if (myKeys.add_key(FIVE)) Serial.println("Error adding key");
  if (myKeys.add_key(ZERO)) Serial.println("Error adding key");
  if (myKeys.add_key(ADD)) Serial.println("Error adding key");
  if (myKeys.add_key(NO_KEY)) Serial.println("Error adding key");
  if (myKeys.add_key(THREE)) Serial.println("Error adding key");
  if (myKeys.add_key(EQUAL)) Serial.println("Error adding key");
  if (myKeys.add_key(OnC)) Serial.println("Error adding key");
  if (myKeys.add_key(PIE)) Serial.println("Error adding key");
  if (myKeys.add_key(X2)) Serial.println("Error adding key");
  if (myKeys.add_key(COS)) Serial.println("Error adding key");

}

void loop() {
  curr_millis = millis();

  switch (state) {
    case START_SCAN_CYCLE:
      current_key = myKeys.get_key();
      state = NEXT_ROW_SCAN;
      current_row = 1;
      if (current_key != NO_KEY) { /// 
        Serial.print("New key: ");         ///
        Serial.println(int(current_key));  ///
      } ///
      break;

    // Row scan inactive and previous key press is complete
    // Wait for next row scan
    // This state is skipped for row 8 (because it is always high)
    case NEXT_ROW_SCAN:
      if (ti30.rowState(current_row) == HIGH) state = IS_KEY_IN_THIS_ROW;
      break;

    // Check the keypress queue to see if next key is for this row
    case IS_KEY_IN_THIS_ROW:
      if (current_key == NO_KEY)  {
        state = SKIP_THIS_ROW;
      } else {
        if ( (ti30.getRow(current_key) + 1) != current_row ) {
          state = SKIP_THIS_ROW;
        } else {
          state = START_KEYPRESS;
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
          Serial.println("Waiting a second.");  /// Delete this once done
          prev_millis = curr_millis;            /// Delete this once done
          state = WAIT_A_SECOND;                /// Delete this and uncomment next once done
          ///          state = NEXT_ROW_SCAN
        } else {
          if (current_row < MAX_ROW) state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN;
          else state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8;
        }
      }
      break;

    // Row scan ended, but still want to keep key pressed long enough to
    // get through debounce filter
    case ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN:
      if (current_row < MAX_ROW && ti30.rowState(current_row) == HIGH){
        state = CONTINUE_KEYPRESS;
        ti30.pressKey(current_key);
      }
      break;

    case ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8:
      if (current_row == MAX_ROW && ti30.rowState(current_row - 1) == HIGH) state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN_ROW_8_PART_2;
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
          if (current_row < MAX_ROW) state = NEXT_ROW_SCAN;  // For row 8, there is no row signal
        }
      } else {
        // Special handling for row 8, so need to look when row 1 starts again
        // Skip NEXT_ROW_SCAN state because we are already checking it here
        if (ti30.rowState(1) == HIGH) {
          state = START_SCAN_CYCLE;
        }
      }
      break;

    /// This is just for testing until full state machine implemented.
    case WAIT_A_SECOND:
      if (curr_millis - prev_millis > LOOP_DELAY) {
        state = START_SCAN_CYCLE;
      }
      break;

    default:
      Serial.println("ERROR: Unexpected state.");
      state = START_SCAN_CYCLE;
      break;
  }
}
