/* -----------------------------------------------------------------
   TI-30 Calculator Keypad Simulator
   https://github.com/Andy4495/TI-30-Keypad-Interface

   26-Oct-2023 - Andy4495 - Original

   Library to simulate the keypad of a TI-30 calculator

*/

#include "TI30.h"

TI30::TI30(byte r1, byte r2, byte r3, byte r4, byte r5, byte r6, byte r7,
      byte c1, byte c2, byte c3, byte c4, byte c5) {
      
  _row[0] = r1;
  _row[1] = r2;
  _row[2] = r3;
  _row[3] = r4;
  _row[4] = r5;
  _row[5] = r6;
  _row[6] = r7;
  _col[0] = c1; 
  _col[1] = c2; 
  _col[2] = c3; 
  _col[3] = c4; 
  _col[4] = c5; 
}

void TI30::begin() {
  byte i;
  for (i = 0; i < 7; i++) {
    pinMode(_row[i], INPUT);
  }
  for (i = 0; i < 5; i++) {
    digitalWrite(_col[i], LOW);
    pinMode(_col[i], OUTPUT);
  }
}

void TI30::pressKey(KEYNAME k) {
  if (k < SPECIALKEYS) {
    digitalWrite(_col[getCol(k)], HIGH);
  }
}

void TI30::releaseKey(KEYNAME k) {
  if (k < SPECIALKEYS) {
    digitalWrite(_col[getCol(k)], LOW);
  }
}

// This is a BLOCKING function -- it should generally only be used for testing
// Normal functionality should be implemented with a state machine
// Note that pressKeyBlocking() will not work on ROW 8 (1/x, x^2, ... ON/C) because the row is always high
void TI30::pressKeyBlocking(KEYNAME k) {
  if ( (k < SPECIALKEYS) || (k < INV) ) {
    while (rowState(getRow(k)) == LOW) ;  // Empty statement; loop until row goes HIGH
    pressKey(k);
    while (rowState(getRow(k)) == HIGH);  // Empty statement; loop until row goes LOW
  }
}

// getRow() returns a 0-relative array index from 0 to 7
byte TI30::getRow(KEYNAME k) {
  return 8 - (k / 5) - 1;  // Subtract 1 since row array is zero-relative
}

// getCol() returns a 0-relative array index from 0 to 4
byte TI30::getCol(KEYNAME k) {
  return (k % 5); 
}

// "r" is a value from 1 to 8 (i.e., it matches the 1-relative counting used by the documentation)
byte TI30::rowState(byte r) {
  byte rs = digitalRead(_row[r - 1]);
  return rs;
}

keylist::keylist(uint16_t size) {
  keys = new KEYNAME[size];
  push_position = 0;
  pop_position = 0;
  keys_in_list = 0;
  list_size = size;
}

// returns 0 on success; 1 if list is full
int keylist::add_key(KEYNAME k) {
  int retval;
  if (keys_in_list < list_size) {
    keys[push_position] = k;
    keys_in_list++;
    ++push_position;
    retval = 0;
  } else {
    retval = 1;
  }
  return retval;
}

// returns NOKEY if list is empty
KEYNAME keylist::get_key(){
  KEYNAME retval;
  if (pop_position < keys_in_list) {
    retval = keys[pop_position++];
  } else {
    retval = NOKEY;
  }
  return retval;
}

void keylist::clear_list() {
  keys_in_list = 0;
  push_position = 0;
  pop_position = 0;
}

void keylist::start_list() {
  pop_position = 0;
}
