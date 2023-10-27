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
  for (i = 0; i < 8; i++) {
    pinMode(i, INPUT);
  }
  for (i = 0; i < 5, i++) {
    digitalWrite(i, LOW);
    pinMode(i, OUTPUT);
  }
}

void TI30::pressKey(KEYNAME k) {

}

void TI30::releaseKey(KEYNAME k) {

}

void TI30::pressKey(byte row, byte col) {

}

void TI30::releaseKey(byte row, byte col) {

}

byte TI30::getRow(KEYNAME k) {
  return 8 - (k / 5) - 1;  // Subtract 1 since row array is zero-relative
}

byte TI30::getCol(KEYNAME k) {
  return (k % 5) - 1; 
}
