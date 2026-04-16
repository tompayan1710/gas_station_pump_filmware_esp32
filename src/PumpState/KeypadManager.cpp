#include "KeypadManager.h"
#include "../Config.h"

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[KEYPAD_ROWS] = {
  KEYPAD_ROW_PINS[0],
  KEYPAD_ROW_PINS[1],
  KEYPAD_ROW_PINS[2],
  KEYPAD_ROW_PINS[3]
};
byte colPins[KEYPAD_COLS] = {
  KEYPAD_COL_PINS[0],
  KEYPAD_COL_PINS[1],
  KEYPAD_COL_PINS[2]
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
