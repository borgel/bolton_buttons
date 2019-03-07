#pragma once

#include <stdint.h>

// 9 keys (including esc)
#define NUM_DIRECT_BUTTONS 5  // buttons connected directly to the teensy
#define NUM_INDIRECT_BUTTONS 12 // buttons on the IO expander
#define NUM_KNOBS 4

#define KEYMAP_SIZE 9   // 9 from original code FIXME change

typedef void (*specialCallback)(bool const wasPress);

// our one special CB is here, so we can refer to it in the assignment table. Fxn body is in the main ino
void modeButtonCB(bool const wasPress);
void encoderButtonCB(bool const wasPress);

typedef struct {
  int pin;
  int assignmentMapIndex;     // used to index into the selected keymap to activate an element
  specialCallback special;
} ButtonAssignment;

// TODO separate pin from key
ButtonAssignment button_assignments[NUM_DIRECT_BUTTONS] = {
  //pin, keymap index, related LED chain offset
  {15, 0},   //knob 1
  {16, 1},   //knob 2
  {17, 2},   //knob 3
  {18, 3},   //knob 4
  {19, 4},   //big center
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

// keymaps for each button. ORDER MATTERS! When a key is pressed above
// it will index into this array and invoke a key
KeymapAssignment const keymapLayout[KEYMAP_SIZE] = {
  //esc
  {"ESC",
    .press={KS_NO_MODIFIER, KEY_ESC, "ESC"},
    .decrement={MODIFIERKEY_CTRL, KEY_Z, "Undo"},
    .increment={MODIFIERKEY_CTRL, KEY_Y, "Redo"},
  },

  //f1
  {"Grid",
    .press={KS_NO_MODIFIER,        KEY_HOME, "Home"},
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
    .press={KS_NO_MODIFIER,        KEY_X, "Start Track"},    // start new trace
    .decrement={MODIFIERKEY_CTRL,  KEY_W, "Track Width -"},
    .increment={KS_NO_MODIFIER,    KEY_W, "Track Width +"}, 
  },
  //f4
  {"Move",
    .press={KS_NO_MODIFIER,        KEY_M, "Move"},
    .decrement={KS_NO_MODIFIER,    KEY_R, "Rotate+"},
    .increment={MODIFIERKEY_SHIFT, KEY_R, "Rotate-"},
  },
  //f5
  {"Recenter",
    .press={MODIFIERKEY_CTRL,      KEY_F5, "Region Zoom"},
    .decrement={KS_NO_MODIFIER,    KEY_F2, "Zoom Out"},
    .increment={KS_NO_MODIFIER,    KEY_F1, "Zoom In"},
  },
  //f6
  {"Delete",
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
KeymapAssignment const keymapTest[KEYMAP_SIZE] = {
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

typedef struct {
  const char *name;
  int keymapLen;
  KeymapAssignment const * const keymap;
  KeymapAssignment defaultKnob;
} KeymapConfig;

KeymapAssignment defaultDialLayout = {
  "Dial Zoom", .press = {},
  .decrement={KS_NO_MODIFIER,    KEY_F2, "Zoom Out"},
  .increment={KS_NO_MODIFIER,    KEY_F1, "Zoom In"},
};
KeymapAssignment defaultDialIdle = {
  "Dial Zoom", .press = {},
  .decrement={KS_NO_MODIFIER,    KEY_UP, "Scroll Up"},
  .increment={KS_NO_MODIFIER,    KEY_DOWN, "Scroll Down"},
};

// the master structure of maps
KeymapConfig const allKeymaps[] = {
  {"KiCAD: Schematic", KEYMAP_SIZE, keymapTest, defaultDialLayout},
  {"KiCAD: Layout", KEYMAP_SIZE, keymapLayout, defaultDialLayout},
  {"Idle", 0, keymapLayout, defaultDialIdle},
  {NULL},   // end flag
};
