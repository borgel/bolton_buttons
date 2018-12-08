#pragma once

#include <stdint.h>

// 9 keys (including esc)
#define NUM_NORMAL_KEYS (9)
// add rotary, bonus switch
#define NUM_BUTTONS (NUM_NORMAL_KEYS+1+1)

typedef void (*specialCallback)(bool const wasPress);

// our one special CB is here, so we can refer to it in the assignment table. Fxn body is in the main ino
void modeButtonCB(bool const wasPress);
void encoderButtonCB(bool const wasPress);

typedef struct {
  int pin;
  int assignmentMapIndex;     // this order matters! do not change it casually!
  int ledIndex;
  specialCallback special;
} ButtonAssignment;

// TODO separate pin from key
ButtonAssignment button_assignments[NUM_BUTTONS] = {
  //pin, keymap index, related LED
  {2, 5, 7},   //5
  {3, 7, 0},   //7
  {4, 6, 6},   //6
  {5, 2, 5},   //2
  {6, 0, 0},   //esc
  {7, 1, 8},   //1
  {8, 4, 2},   //4
  {9, 8, 1},   //8
  {10, 3,3},   //3
  
  {16, -1, 0, modeButtonCB},  // bonus button
  {15, -1, 0, encoderButtonCB},  // encoder switch
};

typedef struct {
  int32_t modifier;
  uint32_t key;
  const char *name;
} KeymapAssignment;

typedef struct {
  const char *name;
  int keymapLen;
  KeymapAssignment const * const keymap;
} KeymapConfig;

// keymaps for each button. ORDER MATTERS! When a key is pressed above
// it will index into this array and invoke a key
KeymapAssignment keymapLayout[NUM_NORMAL_KEYS] = {
  {-1,                  KEY_ESC, "ESC"},    //esc
  {MODIFIERKEY_SHIFT,   KEY_N, "Grid -"},      //f1
  {-1,                  KEY_V, "Layer Swap"},      //f2
  {-1,                  KEY_X, "Start Track"},      //f3
  {MODIFIERKEY_CTRL ,   KEY_W, "Track Width -"},   //f4
  {-1,                  KEY_N, "Grid +"},   //f5
  {-1,                  KEY_DELETE, "Delete"},   //f6
  {-1,                  KEY_M, "Move"},   //f7
  {-1,                  KEY_W, "Track Width +"},   //f8
};
KeymapAssignment keymapTest[NUM_NORMAL_KEYS] = {
  {-1,                  KEY_E, "ESC"},    //esc
  {-1,                  KEY_1, "1"},     //f1
  {-1,                  KEY_2, "2"},     //f2
  {-1,                  KEY_3, "3"},     //f3
  {-1,                  KEY_4, "4"},     //f4
  {-1,                  KEY_5, "5"},     //f5
  {-1,                  KEY_6, "6"},     //f6
  {-1,                  KEY_7, "7"},     //f7
  {-1,                  KEY_8, "8"},     //f8
};

// the master structure of maps
KeymapConfig allKeymaps[] = {
  {"KiCAD: Schematic", NUM_NORMAL_KEYS, keymapTest},
  {"KiCAD: Layout", NUM_NORMAL_KEYS, keymapLayout},
  {NULL},   // end flag
};
