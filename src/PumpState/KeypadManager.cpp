#include "KeypadManager.h"
#include "../Config.h"

char keys[KEYPAD_ROWS][KEYPAD_COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[KEYPAD_ROWS] = {14, 32, 33, 26};
byte colPins[KEYPAD_COLS] = {27, 13, 25};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);