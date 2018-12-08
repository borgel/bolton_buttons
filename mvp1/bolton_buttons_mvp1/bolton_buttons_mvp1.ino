

#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>

#include <oled.h>

#include <usb_keyboard.h>

#include "buttonmap.h"

static int currentKeyConfig;

//TODO package
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
OLED display = OLED(23, 22 ,NO_RESET_PIN,0x3C,SCREEN_WIDTH,SCREEN_HEIGHT, true);

// TODO package
static Adafruit_DotStar strip = Adafruit_DotStar(9, 12, 14, DOTSTAR_BRG);
static Bounce * buttonBouncers = new Bounce[NUM_BUTTONS];

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
    buttonBouncers[i].interval(25);
    
    Serial.printf("Attached button on pin %d\n", ba->pin);
  }

  strip.begin();
  strip.show();

  //setup display
  display.begin();
  display.print("Potatoe Is Nice");
  display.display();
  
  switchKeyconfig(true);

  Serial.println("Done with setup");
}

void loop() {
  //check for key flags, and send data
  for(int i = 0; i < NUM_BUTTONS; i++) {
    Bounce * b = &buttonBouncers[i];
    b->update();
    if(b->rose()) {
      keyEvent(&button_assignments[i], false);
    }
    else if(b->fell()) {
      keyEvent(&button_assignments[i], true);
    }
  }
}

void keyEvent(ButtonAssignment const * const b, bool const wasPress) {
  // if this was a special key, fire the CB and return
  if(b->special) {
    return b->special(wasPress);
  }

  // normal key
  KeymapAssignment const * const ka = getKeymappingForKey(b);
  if(wasPress) {
    strip.setPixelColor(b->ledIndex, 255, 127, 0);
    Keyboard.press(ka->key);
  }
  else {
    strip.setPixelColor(b->ledIndex, 0, 0, 0);
    Keyboard.release(ka->key);
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

  Serial.printf("Selected keymap to %d (%s)\n", currentKeyConfig, allKeymaps[currentKeyConfig].name);

  // now do whatever to init this map
  display.clear();
  display.printf("%s\n", allKeymaps[currentKeyConfig].name);
  display.display();
}
