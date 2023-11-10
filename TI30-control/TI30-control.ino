/* -----------------------------------------------------------------
   TI-30 Calculator Keypad Simulator
   https://github.com/Andy4495/TI-30-Keypad-Interface

   08-Nov-2023 - Andy4495 - Original

   Sketch to simulate the keypad of a TI-30 calculator

*/

#include "TI30.h"

//     TI30(byte r1, byte r2, byte r3, byte r4, byte r5, byte r6, byte r7,
//     byte c1, byte c2, byte c3, byte c4, byte c5);
TI30 ti30(15, 19, 10, 12, 14, 13, 18,
          10, 9, 8, 7, 6);

keylist myKeys(32);

#define LOOP_DELAY 1000
#define KEYBOUNCE_CYCLES 1

unsigned long prev_millis = 0;
unsigned long curr_millis;
bool scan_trigger = false;
int bounce_count = 0;
KEYNAME current_key = NO_KEY;

// States
enum STATE {
  ROW_SCAN_OFF,
  GET_NEXT_KEYPRESS,
  START_KEYPRESS,
  CONTINUE_KEYPRESS,
  ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN,
  WAIT_A_SECOND  /// Delete this once done
};

STATE state = ROW_SCAN_OFF;

void setup() {
  Serial.begin(9600);
  Serial.println("TI-30 Calculator Keyboard v3");
  ti30.begin();
  if (myKeys.add_key(ONE))    Serial.println("Error adding key");
  if (myKeys.add_key(TWO))    Serial.println("Error adding key");
  if (myKeys.add_key(THREE))  Serial.println("Error adding key");
  if (myKeys.add_key(ADD)) Serial.println("Error adding key");
  if (myKeys.add_key(NO_KEY)) Serial.println("Error adding key");
  if (myKeys.add_key(THREE))  Serial.println("Error adding key");
  if (myKeys.add_key(TWO))    Serial.println("Error adding key");
  if (myKeys.add_key(ONE))    Serial.println("Error adding key");
}

void loop() {
  curr_millis = millis();

  switch (state) {
    // Row scan inactive and previous key press is complete
    // Wait for next row scan
    case ROW_SCAN_OFF:
      if (ti30.rowState(2) == HIGH) state = GET_NEXT_KEYPRESS;
      break;

    // Row scan is active

    // Check the keypress queue to see if next key is for this row
    /// This is a placeholder state for now since all keypresses are for this row **********
    case GET_NEXT_KEYPRESS:
      current_key = myKeys.get_key();
      Serial.print("New key: ");    ///
      Serial.println(int(current_key)); ///
      state = START_KEYPRESS;
      break;

    // Activate the column corresponding to the key
    /// Need more code to select correct column based on key ***********
    /// Hardcoded for now **********
    case START_KEYPRESS:
      ti30.pressKey(current_key);
      state = CONTINUE_KEYPRESS;
      /// break;   /// Intentionally let this fall through for now ******

    // Row scan is active
    case CONTINUE_KEYPRESS:
      if (ti30.rowState(2) == LOW) {
        ti30.releaseKey(current_key);
        bounce_count++;
        if (bounce_count > KEYBOUNCE_CYCLES) {
          bounce_count = 0;
          Serial.println("Waiting a second.");  /// Delete this once done
          prev_millis = curr_millis;            /// Delete this once done
          state = WAIT_A_SECOND;                /// Delete this and comment next once done
          ///          state = ROW_SCAN_OFF
        } else {
          state = ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN;
        }
      }
      break;

    // Row scan ended, but still want to keep key pressed long enough to
    // get through debounce filter
    case ACTIVE_KEYPRESS_WAITING_FOR_NEXT_SCAN:
      if (ti30.rowState(2) == HIGH) {
        state = CONTINUE_KEYPRESS;
        ti30.pressKey(current_key);
      }
      break;

    /// This is just for testing until full state machine implemented.
    case WAIT_A_SECOND:
      if (curr_millis - prev_millis > LOOP_DELAY) {
        state = ROW_SCAN_OFF;
      }
      break;

    default:
      Serial.println("ERROR: Unexpected state.");
      state = ROW_SCAN_OFF;
      break;
  }
}
