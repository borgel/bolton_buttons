#include "TCA9555.h"

#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <SPI.h>

#include <usb_keyboard.h>
#include "SmartKnob.h"

#include "buttonmap.h"

static int currentKeyConfig = 0;

// TODO package
static Adafruit_DotStar strip = Adafruit_DotStar(3, 12, 14, DOTSTAR_BRG);
static Bounce * buttonBouncers = new Bounce[NUM_DIRECT_BUTTONS];

//static Encoder knob1(11, 10);
//static Encoder knob2(9, 8);
//static Encoder knob3(7, 6);
//static Encoder knob4(5, 4);

static SmartKnob knob1(11, 10);
//static SmartKnob * knobs = new SmartKnob[NUM_KNOBS];
static SmartKnob knobs[NUM_KNOBS] = {
  SmartKnob(11, 10),
  SmartKnob(9, 8),
  SmartKnob(7, 6),
  SmartKnob(5, 4),
};

static KeymapAssignment const * pressedKey;

void setup() {
  // wait for USB enumeration
  delay(1000);
  Serial.begin(9600);
  Serial.println("Booted...");

  // attach bouncer to all directly connected buttons
  ButtonAssignment *ba;
  for(int i = 0; i < NUM_DIRECT_BUTTONS; i++) {
    ba = &button_assignments[i];

    buttonBouncers[i].attach(ba->pin, INPUT_PULLUP);
    buttonBouncers[i].interval(40);
    
    Serial.printf("Attached button on direct pin %d\n", ba->pin);
  }

  strip.begin();
  strip.show();
  
  switchKeyconfig();

  Serial.println("Done with setup");
}

static long lastKnob = 0;
void loop() {
  //check for key flags, and send data
  // set the knob IO to low for just a moment to sense

  for(int i = 0; i < NUM_DIRECT_BUTTONS; i++) {
    Bounce * b = &buttonBouncers[i];
    b->update();
    if(b->rose()) {
      keyPressEvent(&button_assignments[i], false);
    }
    else if(b->fell()) {
      keyPressEvent(&button_assignments[i], true);
    }
  }

  // look for a knob change and dispatch events
  //TODO iterate through knobs and look for change
  /*
  if(knob1.didChange()) {
    Serial.println("changed");
  }
  */
  for(int i = 0; i < NUM_KNOBS; i++) {
    SmartKnob * k = &knobs[i];
    if(k->didChange()) {
      Serial.printf("k%d change\n", i);
    }
  }
  /*
  if(knob.read() != lastKnob) {
    long prevKnob = lastKnob;
    lastKnob = knob.read();

    // only dispatch an event on %4 (that matches the HW detents)
    if(lastKnob % 4 == 0) {
      // clockwise is more negative
      knobEvent(lastKnob - prevKnob < 0);
    }
  }
  */
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

    // mark this key as being pressed
    pressedKey = ka;
  }
  else {
    safeKeyboardRelease(&ka->press);

    // unmark this key as being pressed
    pressedKey = NULL;
  }
  strip.show();
}

void knobEvent(bool const wasIncrement) {
  if(pressedKey) {
    // release this key's pressed state
    safeKeyboardRelease(&pressedKey->press);
    
    if(wasIncrement){
      safeKeyboardPress(&pressedKey->increment);
      //TODO need delay?
      safeKeyboardRelease(&pressedKey->increment);
    }
    else {
      safeKeyboardPress(&pressedKey->decrement);
      //TODO need delay?
      safeKeyboardRelease(&pressedKey->decrement);
    }
  }
  else {
    // layout-based default knob
    KeymapAssignment const * const ka = &allKeymaps[currentKeyConfig].defaultKnob;
    if(wasIncrement) {
      safeKeyboardPress(&ka->increment);
      safeKeyboardRelease(&ka->increment);
    }
    else {
      safeKeyboardPress(&ka->decrement);
      safeKeyboardRelease(&ka->decrement);
    }
  }
}

void modeButtonCB(bool const wasPress) {
  if(wasPress) {
    switchKeyconfig();
  }
}

void encoderButtonCB(bool const wasPress) {
  Serial.printf("Encoder button press was %d", wasPress);
}

const KeymapAssignment * const getKeymappingForKey(ButtonAssignment const * const butt) {
  if(butt->assignmentMapIndex == -1) {
    Serial.println("ERROR: Tried to get keymap for key without index");
    while(1);
  }
  // the current group of key assignments
  KeymapAssignment const * const ka = allKeymaps[currentKeyConfig].keymap;
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
  Serial.printf("PKey %d\n", ks->key);
  return;
  
  if(ks->modifier != KS_NO_MODIFIER) {
    Keyboard.press(ks->modifier);
  }
  if(ks->key != KS_INACTIVE) {
    Keyboard.press(ks->key);
  }
}void safeKeyboardRelease(KeyShortcut const * const ks) {
  //FIXME rm
  Serial.printf("RKey %d\n", ks->key);
  return;
  
  if(ks->modifier != KS_NO_MODIFIER) {
    Keyboard.release(ks->modifier);
  }
  if(ks->key != KS_INACTIVE) {
    Keyboard.release(ks->key);
  }
}
