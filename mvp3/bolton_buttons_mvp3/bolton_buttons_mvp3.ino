#include <i2c_t3.h>

#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <SPI.h>

#include <usb_keyboard.h>
#include "ExpanderButton.h"
#include "SmartKnob.h"

#include "buttonmap.h"

static int currentKeyConfig = 0;

// TODO package
static Adafruit_DotStar strip = Adafruit_DotStar(3, 12, 14, DOTSTAR_BRG);
static Bounce * buttonBouncers = new Bounce[NUM_DIRECT_BUTTONS];

static SmartKnob knobs[NUM_KNOBS] = {
  SmartKnob(11, 10),
  SmartKnob(9, 8),
  SmartKnob(7, 6),
  SmartKnob(5, 4),
};

void setup() {
  // wait for USB enumeration
  delay(1000);
  Serial.begin(9600);
  Serial.println("Booted...");

  // attach bouncer to all directly connected buttons
  ButtonAssignment const *ba;
  for(int i = 0; i < NUM_DIRECT_BUTTONS; i++) {
    ba = &button_assignments_direct[i];

    buttonBouncers[i].attach(ba->pin, INPUT_PULLUP);
    buttonBouncers[i].interval(40);
    
    Serial.printf("Attached button on direct pin %d\n", ba->pin);
  }

  strip.begin();
  strip.show();
  
  Wire1.begin();
  ep_Init(2, expandedKeyPress);

  // init keymaps and state vars
  switchKeyconfig();

  Serial.println("Done with setup");
}

void loop() {
  // let the IO expander check for changes
  ep_HandleChanges();

  // handle all directly connected buttons
  for(int i = 0; i < NUM_DIRECT_BUTTONS; i++) {
    Bounce * b = &buttonBouncers[i];
    b->update();
    if(b->rose()) {
      //FIXME rm
      Serial.printf("Int %d press? 0\n", i);
      keyPressEvent(&button_assignments_direct[i], false);
    }
    else if(b->fell()) {
      //FIXME rm
      Serial.printf("Int %d press? 1\n", i);
      keyPressEvent(&button_assignments_direct[i], true);
    }
  }

  // look for a knob change and dispatch events
  for(int i = 0; i < NUM_KNOBS; i++) {
    SmartKnob * k = &knobs[i];
    if(k->didChange()) {
      knobEvent(i, k->getChange() < 0);
    }
  }
}

// invoked every time a button on the IO expander changes
void expandedKeyPress(uint8_t const pin, bool const wasPress) {
  Serial.printf("Exp Btn %d press? %d\n", pin, wasPress);
  
  keyPressEvent(&button_assignments_indirect[pin], wasPress);
}

void keyPressEvent(ButtonAssignment const * const b, bool const wasPress) {
  // if this was a special key, fire the CB and return
  if(b->special) {
    return b->special(wasPress);
  }

  // normal key
  KeymapAssignment const * const ka = getKeymappingForKey(b);
  if(wasPress) {
    safeKeyboardPress(&ka->press);
  }
  else {
    safeKeyboardRelease(&ka->press);
  }
  strip.show();
}

void knobEvent(uint8_t const index, bool const wasIncrement) {
  Serial.printf("k%d change %d\n", index, wasIncrement);
  
  KeymapAssignment const * const ka = &allKeymaps[currentKeyConfig].knobmap[index];
  
  if(wasIncrement) {
    safeKeyboardPress(&ka->increment);
    safeKeyboardRelease(&ka->increment);
  }
  else {
    safeKeyboardPress(&ka->decrement);
    safeKeyboardRelease(&ka->decrement);
  }
}

void modeButtonCB(bool const wasPress) {
  if(wasPress) {
    switchKeyconfig();
  }
}

void centerButtonCB(bool const wasPress) {
  KeymapAssignment const esc = {"ESC",
    .press={KS_NO_MODIFIER, KEY_ESC, "ESC"},
  };
  
  if(wasPress) {
    safeKeyboardPress(&esc.press);
  }
  else {
    safeKeyboardRelease(&esc.press);
  }
}

const KeymapAssignment * const getKeymappingForKey(ButtonAssignment const * const butt) {
  if(butt->assignmentMapIndex == -1) {
    Serial.println("ERROR: Tried to get keymap for key without index");
    while(1);
  }
  
  KeymapAssignment const * ka;
  if(butt->isKnob) {
    ka = allKeymaps[currentKeyConfig].knobmap;
  }
  else {
    // the current group of key assignments
    ka = allKeymaps[currentKeyConfig].buttonmap;
  }
  
  // get the assignment for this key
  return &ka[butt->assignmentMapIndex];
}

void switchKeyconfig() {
  currentKeyConfig++;
  if(allKeymaps[currentKeyConfig].name == NULL) {
    currentKeyConfig = 0;
  }

  KeymapConfig const * const kc = &allKeymaps[currentKeyConfig];
  Serial.printf("Selected keymap to %d (%s)\n", currentKeyConfig, kc->name);
}

// perform all safety checks and press or release a key with modifiers
void safeKeyboardPress(KeyShortcut const * const ks) {
  //FIXME rm
  //Serial.printf("PKey %d %d\n", ks->modifier, ks->key);
  //return;
  
  if(ks->modifier != KS_NO_MODIFIER) {
    Keyboard.press(ks->modifier);
  }
  if(ks->key != KS_INACTIVE) {
    Keyboard.press(ks->key);
  }
}void safeKeyboardRelease(KeyShortcut const * const ks) {
  //FIXME rm
  //Serial.printf("RKey %d %d\n", ks->modifier, ks->key);
  //return;
  
  if(ks->modifier != KS_NO_MODIFIER) {
    Keyboard.release(ks->modifier);
  }
  if(ks->key != KS_INACTIVE) {
    Keyboard.release(ks->key);
  }
}
