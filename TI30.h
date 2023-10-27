/* -----------------------------------------------------------------
   TI-30 Calculator Keypad Simulator
   https://github.com/Andy4495/TI-30-Keypad-Interface

   26-Oct-2023 - Andy4495 - Original

   Library to simulate the keypad of a TI-30 calculator

*/

#ifndef TI30_H
#define TI30_H

#include "Arduino.h"

enum KEYNAME {
  RECIP, X-2,   SQRT,  OFF,   OnC,  // RECIP is 1/x, X-2 is x^2
  INV,   SIN,   COS,   TAN,   DRG,
  K,     EE,    LOG,   LNX,   Y-X,  // Y-X is Y^X
  PI,    PERC,  LEFT,  RIGHT, DIV,  // PERC is %, LEFT is (, RIGHT is ), DIV is /
  STO,   SEVEN, EIGHT, NINE,  MUL,  // MUL is *
  RCL,   FOUR,  FIVE,  SIX,   SUB,  // SUB is -
  SUM,   ONE,   TWO,   THREE, ADD,
  EXC,   ZERO,  DOT,   PLUS-, EQUAL  // PLUS- is +/-
}

// ROW is 8 - (KEYNAME / 5)
// COL is KEYNAME % 5

class TI30 {
public:

  // Define the pin numbers for the row and column sense lines on the keypad
  // Rows are numbered bottom-up ('0' is in row 1)
  // - Row 8 is not connected since it is always high (connected to Vss/9V)
  // Columns are numbered left-right ('EXC' is in column 1)
  TI30(byte r1, byte r2, byte r3, byte r4, byte r5, byte r6, byte r7,
       byte c1, byte c2, byte c3, byte c4, byte c5);

  void begin();
  void pressKey(KEYNAME k);
  void releaseKey(KEYNAME k);
  void pressKey(byte row, byte col);
  void releaseKey(byte row, byte col);

private:
  byte _row[7];
  byte _col[5];
  byte getRow(KEYNAME k); 
  byte getCol(KEYNAME k);
};

#endif
