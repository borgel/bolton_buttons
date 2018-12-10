

#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>

#include <oled.h>

#include <usb_keyboard.h>
#include <Encoder.h>
#include "SoftPWM.h"

#include "buttonmap.h"

static int currentKeyConfig;

//TODO package
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
OLED display = OLED(23, 22 ,NO_RESET_PIN,0x3C,SCREEN_WIDTH,SCREEN_HEIGHT, true);

// TODO package
static Adafruit_DotStar strip = Adafruit_DotStar(9, 12, 14, DOTSTAR_BRG);
static Bounce * buttonBouncers = new Bounce[NUM_BUTTONS];

static Encoder knob(20, 21);

void setup() {
  // wait for USB enumeration
  delay(1000);
  Serial.begin(9600);
  Serial.println("Booted...");
  
  // attach bouncer to all buttons
  ButtonAssignment *ba = &button_assignments[0];
  for(int i = 0; i < NUM_BUTTONS; i++) {
    ba = &button_assignments[i];

    buttonBouncers[i].attach(ba->pin, INPUT_PULLUP);
    buttonBouncers[i].interval(40);
    
    Serial.printf("Attached button on pin %d\n", ba->pin);
  }

  strip.begin();
  strip.show();

  //setup display
  display.begin();
  display.print("Potatoe Is Nice");
  display.display();
  
  switchKeyconfig(true);

  // knob LEDs
  SoftPWMBegin(SOFTPWM_INVERTED);
  SoftPWMSet(17, 0);
  SoftPWMSet(18, 0);
  SoftPWMSet(19, 0);
  SoftPWMSetFadeTime(ALL, 100, 100);
  setKnobLEDWhite(0);

  Serial.println("Done with setup");
}

static long lastKnob = 0;
void loop() {
  //check for key flags, and send data
  // set the knob IO to low for just a moment to sense
  
  for(int i = 0; i < NUM_BUTTONS; i++) {
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
  if(knob.read() != lastKnob) {
    lastKnob = knob.read();

    // only dispatch an event on %4 (that matches the HW detents)
    if(lastKnob % 4 == 0) {
      Serial.printf("knob %d\n", lastKnob);

      //TODO send events somewhere
    }
  }
}

void keyPressEvent(ButtonAssignment const * const b, bool const wasPress) {
  // if this was a special key, fire the CB and return
  if(b->special) {
    return b->special(wasPress);
  }

  // normal key
  KeymapAssignment const * const ka = getKeymappingForKey(b);
  if(wasPress) {
    strip.setPixelColor(b->ledIndex, 255, 127, 0);
    setKnobLED(rand() % 100, rand() % 100, rand() % 100);

    if(ka->press.key != KS_INACTIVE) {
      if(ka->press.modifier != KS_NO_MODIFIER) {
        Keyboard.press(ka->press.modifier);
      }
      Keyboard.press(ka->press.key);
    }
  }
  else {
    strip.setPixelColor(b->ledIndex, 0, 0, 0);
    setKnobLEDWhite(0);
    
    if(ka->press.key != KS_INACTIVE) {
      if(ka->press.modifier != KS_NO_MODIFIER) {
        Keyboard.release(ka->press.modifier);
      }
      Keyboard.release(ka->press.key);
    }
  }
  strip.show();
}

void modeButtonCB(bool const wasPress) {
  if(wasPress) {
    switchKeyconfig(true);
  }
}

void encoderButtonCB(bool const wasPress) {
  Serial.printf("Encoder button press was %d", wasPress);
}

const KeymapAssignment * const getKeymappingForKey(ButtonAssignment const * const butt) {
  // the current group of key assignments
  KeymapAssignment const * const ka = allKeymaps[currentKeyConfig].keymap;
  // get the assignment for this key
  return &ka[butt->assignmentMapIndex];
}

void switchKeyconfig(bool increment) {
  currentKeyConfig++;
  if(allKeymaps[currentKeyConfig].name == NULL) {
    currentKeyConfig = 0;
  }

  KeymapConfig const * const kc = &allKeymaps[currentKeyConfig];
  Serial.printf("Selected keymap to %d (%s)\n", currentKeyConfig, kc->name);

  // now do whatever to init this map
  display.clear();
  display.printf("%s\n", kc->name);
  for(int i = 0; i < kc->keymapLen; i++) {
    display.printf("%d:%s\n", i, kc->keymap[i].name);
  }
  display.display();
}

// set each param 0-100%
void setKnobLED(uint8_t r, uint8_t g, uint8_t b) {
  SoftPWMSetPercent(17, r);
  SoftPWMSetPercent(18, g);
  SoftPWMSetPercent(19, b);
}
void setKnobLEDWhite(uint8_t level) {
  setKnobLED(level, level, level);
}
