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
  RECIP, X2,    SQRT,  OFF,   OnC,  // RECIP is 1/x, X2 is x^2
  INV,   SIN,   COS,   TAN,   DRG,
  K,     EE,    LOG,   LNX,   YX,   // YX is Y^X
  PIE,   PERC,  LEFT,  RIGHT, DIV,  // PIE is PI, PERC is %, LEFT is (, RIGHT is ), DIV is /
  STO,   SEVEN, EIGHT, NINE,  MUL,  // MUL is *
  RCL,   FOUR,  FIVE,  SIX,   SUB,  // SUB is -
  SUM,   ONE,   TWO,   THREE, ADD,
  EXC,   ZERO,  DOT,   PLUSMINUS, EQUAL,  // PLUSMINUS is +/-
  NO_KEY = 99                       // Used as a marker to "do nothing"
};

// Calculating array index from KEYNAME enum:
// ROW is 8 - (KEYNAME / 5) - 1   --> 0 - 7
// COL is KEYNAME % 5             --> 0 - 4

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
  void pressKeyBlocking(KEYNAME k);
  byte rowState(byte r);  // "r" is a value from 1 to 8, with row 1 being the bottom calculator row

private:
  byte _row[7];  // Allocate 8 elements, but array index 7 (calculator row 8) is not used
  byte _col[5];
  byte getRow(KEYNAME k); 
  byte getCol(KEYNAME k);
};

class keylist {
public:
  keylist(uint16_t size);
  int add_key(KEYNAME k);  // returns 0 on success; 1 if list is full
  KEYNAME get_key();     // returns NO_KEY if list is empty
  void clear_list();     // clears the list

private:
  uint16_t push_position;
  uint16_t pop_position;
  uint16_t keys_in_list;
  uint16_t list_size;
  KEYNAME* keys;
};

#endif
