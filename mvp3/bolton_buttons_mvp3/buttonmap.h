#pragma once

#include <stdint.h>

// 9 keys (including esc)
#define NUM_DIRECT_BUTTONS 5  // buttons connected directly to the teensy
#define NUM_INDIRECT_BUTTONS 12 // buttons on the IO expander
#define NUM_KNOBS 4

#define KEYMAP_SIZE 9   // 9 from original code FIXME change

typedef void (*specialCallback)(bool const wasPress);

// our one special CBs are here, so we can refer to them in the assignment table. Fxn body is in the main ino
void centerButtonCB(bool const wasPress);
void encoderButtonCB(bool const wasPress);

typedef struct {
  int pin;
  int assignmentMapIndex;     // used to index into the selected keymap to activate an element
  bool isKnob;
  specialCallback special;
} ButtonAssignment;

// TODO separate pin from key
ButtonAssignment const button_assignments_direct[NUM_DIRECT_BUTTONS] = {
  //pin, keymap index, is knob, special
  {15, 0, true},   //knob 1
  {16, 1, true},   //knob 2
  {17, 2, true},   //knob 3
  {18, 3, true},   //knob 4
  {19, 0, false, centerButtonCB},   //big center  . will route to keymap element 0
};

// order matches pin
ButtonAssignment const button_assignments_indirect[NUM_INDIRECT_BUTTONS] = {
  //pin (unused), keymap index
  {0, 0},   // bottom pair, left
  {1, 1},   // bottom pair, right
  {2, 2},   // bottom side, top
  {3, 3},   // bottom side, bottom
  {4, 4},   // top pair, right
  {5, 5},   // top pair, left
  {6, 6},   // below middle, right
  {7, 7},   // below middle, left
  {8, 8},   // top side, top
  {9, 9},   // top side, bottom
  {10, 10}, // above middle, right
  {11, 11}, // above middle, left
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
KeymapAssignment const knobsLayout [NUM_KNOBS] = {
  //knob 1
  {"Grid",
    .press={KS_NO_MODIFIER,        KEY_HOME, "Home"},
    .decrement={MODIFIERKEY_SHIFT, KEY_N, "Grid -"},
    .increment={KS_NO_MODIFIER,    KEY_N, "Grid +"},
  },
  //knob 2
  {"Move",
    .press={KS_NO_MODIFIER,        KEY_M, "Move"},
    .decrement={KS_NO_MODIFIER,    KEY_R, "Rotate+"},
    .increment={MODIFIERKEY_SHIFT, KEY_R, "Rotate-"},
  },
  // knob 3
  {"Trace",
    .press={KS_NO_MODIFIER,        KEY_X, "Start Track"},    // start new trace
    .decrement={MODIFIERKEY_CTRL,  KEY_W, "Track Width -"},
    .increment={KS_NO_MODIFIER,    KEY_W, "Track Width +"}, 
  },
  //knob 4
  {"Recenter",
    .press={MODIFIERKEY_CTRL,      KEY_F5, "Region Zoom"},
    .decrement={KS_NO_MODIFIER,    KEY_F2, "Zoom Out"},
    .increment={KS_NO_MODIFIER,    KEY_F1, "Zoom In"},
  },
};

// it will index into this array and invoke a key
KeymapAssignment const buttonsLayout[NUM_INDIRECT_BUTTONS] = {
  //esc
  {"ESC",
    .press={KS_NO_MODIFIER, KEY_ESC, "ESC"},
    .decrement={MODIFIERKEY_CTRL, KEY_Z, "Undo"},
    .increment={MODIFIERKEY_CTRL, KEY_Y, "Redo"},
  },
  //
  {"Layer Swap",
    .press={KS_NO_MODIFIER, KEY_V, "Layer Swap"},
  },
  //
  {"Delete",
    .press={KS_NO_MODIFIER, KEY_DELETE, "Delete"},
  },
  //
  {"Flip",
    .press={KS_NO_MODIFIER,    KEY_F, "Flip"},
  },
  //
  {"Edit",
    .press={KS_NO_MODIFIER,        KEY_E, "Edit"},
  },
  //
  {"Flip",
    .press={KS_NO_MODIFIER,    KEY_F, "Flip"},
  },
  //
  {"Edit",
    .press={KS_NO_MODIFIER,        KEY_E, "Edit"},
  },
  //
  {"Flip",
    .press={KS_NO_MODIFIER,    KEY_F, "Flip"},
  },
  //
  {"Edit",
    .press={KS_NO_MODIFIER,        KEY_E, "Edit"},
  },
  //
  {"Flip",
    .press={KS_NO_MODIFIER,    KEY_F, "Flip"},
  },
  //
  {"Edit",
    .press={KS_NO_MODIFIER,        KEY_E, "Edit"},
  },
  //
  {"Flip",
    .press={KS_NO_MODIFIER,    KEY_F, "Flip"},
  },
};

typedef struct {
  const char *name;
  int keymapLen;
  KeymapAssignment const * const buttonmap;
  KeymapAssignment const * const knobmap;
} KeymapConfig;

/*
KeymapAssignment const defaultDialLayout = {
  "Dial Zoom", .press = {},
  .decrement={KS_NO_MODIFIER,    KEY_F2, "Zoom Out"},
  .increment={KS_NO_MODIFIER,    KEY_F1, "Zoom In"},
};
KeymapAssignment const defaultDialIdle = {
  "Dial Zoom", .press = {},
  .decrement={KS_NO_MODIFIER,    KEY_UP, "Scroll Up"},
  .increment={KS_NO_MODIFIER,    KEY_DOWN, "Scroll Down"},
};
*/

// the master structure of maps
KeymapConfig const allKeymaps[] = {
  //{"KiCAD: Schematic", KEYMAP_SIZE, keymapTest, defaultDialLayout},
  {"KiCAD: Layout", KEYMAP_SIZE, buttonsLayout, knobsLayout},
  //{"Idle", 0, keymapLayout, defaultDialIdle},
  {NULL},   // end flag
};
