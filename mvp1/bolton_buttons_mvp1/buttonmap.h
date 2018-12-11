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
  //pin, keymap index, related LED chain offset
  {2, 5, 7},   //5
  {3, 7, 0},   //7
  {4, 6, 6},   //6
  {5, 2, 5},   //2
  {6, 0, 0},   //esc
  {7, 1, 8},   //1
  {8, 4, 2},   //4
  {9, 8, 1},   //8
  {10, 3,3},   //3

  // special keys should come after normal ones
  {16, -1, 0, modeButtonCB},  // bonus button
  {15, -1, 0, encoderButtonCB},  // encoder switch
};

#define KS_INACTIVE (0)
#define KS_NO_MODIFIER (-1)

typedef struct {
  int32_t modifier;
  int key;
  const char *name;
} KeyShortcut;

typedef struct {
  const char *name;
  KeyShortcut press;
  KeyShortcut const decrement;
  KeyShortcut const increment;
} KeymapAssignment;

typedef struct {
  const char *name;
  int keymapLen;
  KeymapAssignment const * const keymap;
} KeymapConfig;

// keymaps for each button. ORDER MATTERS! When a key is pressed above
// it will index into this array and invoke a key
KeymapAssignment const keymapLayout[NUM_NORMAL_KEYS] = {
  //esc
  {"ESC",
    .press={KS_NO_MODIFIER, KEY_ESC, "ESC"},
    .decrement={MODIFIERKEY_CTRL, KEY_Z, "Undo"},
    .increment={MODIFIERKEY_CTRL, KEY_Y, "Redo"},
  },

  //f1
  {"Grid",
    .press={},
    .decrement={MODIFIERKEY_SHIFT, KEY_N, "Grid -"},
    .increment={KS_NO_MODIFIER,    KEY_N, "Grid +"},
  },

  //f2
  {"Layer Swap",
    .press={KS_NO_MODIFIER, KEY_V, "Layer Swap"},
    .decrement={},
    .increment={},
  },
  //f3
  {"Trace",
    .press={KS_NO_MODIFIER,       KEY_X, "Start Track"},    // start new trace
    .decrement={MODIFIERKEY_CTRL, KEY_W, "Track Width -"},
    .increment={KS_NO_MODIFIER,   KEY_W, "Track Width +"}, 
  },
  //f4
  {"Move",
    .press={KS_NO_MODIFIER,        KEY_M, "Move"},
    .decrement={KS_NO_MODIFIER,    KEY_R, "Rotate+"},
    .increment={MODIFIERKEY_SHIFT, KEY_R, "Rotate-"},
  },
  //f5
  {"f5",
    .press={},
    .decrement={},
    .increment={},
  },
  //f6
  {"f6",
    .press={KS_NO_MODIFIER, KEY_DELETE, "Delete"},
    .decrement={},
    .increment={},
  },
  //f7
  {"Delete",
    .press={KS_NO_MODIFIER,    KEY_F, "Flip"},
    .decrement={},
    .increment={},
  },
  //f8
  {"Edit",
    .press={KS_NO_MODIFIER,        KEY_E, "Edit"},
    .decrement={},
    .increment={},
  },
};
KeymapAssignment const keymapTest[NUM_NORMAL_KEYS] = {
  {"ESC",
    .press={KS_NO_MODIFIER, KEY_E, "ESC"},
    .decrement={},
    .increment={},
  },

  //f1
  {"f1",
    .press={KS_NO_MODIFIER, KEY_1, "1"},
    .decrement={KS_NO_MODIFIER, KEY_1, "1-"},
    .increment={KS_NO_MODIFIER, KEY_1, "1+"},
  },
  //f2
  {"f2",
    .press={KS_NO_MODIFIER, KEY_2, "2"},
    .decrement={},
    .increment={},
  },
  {"f3",
    .press={KS_NO_MODIFIER, KEY_3, "3"},
    .decrement={},
    .increment={},
  },
  {"f4",
    .press={KS_NO_MODIFIER, KEY_4, "4"},
    .decrement={},
    .increment={},
  },
  {"f5",
    .press={KS_NO_MODIFIER, KEY_5, "5"},
    .decrement={},
    .increment={},
  },
  {"f6",
    .press={KS_NO_MODIFIER, KEY_6, "6"},
    .decrement={},
    .increment={},
  },
  {"f7",
    .press={KS_NO_MODIFIER, KEY_7, "7"},
    .decrement={},
    .increment={},
  },
  {"f8",
    .press={KS_NO_MODIFIER, KEY_8, "8"},
    .decrement={},
    .increment={},
  },
};


// the master structure of maps
KeymapConfig const allKeymaps[] = {
  {"KiCAD: Schematic", NUM_NORMAL_KEYS, keymapTest},
  {"KiCAD: Layout", NUM_NORMAL_KEYS, keymapLayout},
  {NULL},   // end flag
};
