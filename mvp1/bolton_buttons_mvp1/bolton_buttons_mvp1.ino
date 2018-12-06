

#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>

#include <oled.h>

#include <usb_keyboard.h>

#include "buttonmap.h"

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

  //TODO interrupt config

  //Keyboard.print("a");

  strip.begin();
  strip.show();

  //setup display
  display.begin();
  display.print("Potatoe Is Nice");
  display.display();

  //FIXME rm
  Serial.println("Done with setup");
}

void loop() {
  //check for key flags, and send data
  for(int i = 0; i < NUM_BUTTONS; i++) {
    Bounce * b = &buttonBouncers[i];
    b->update();
    if(b->rose()) {
      Serial.printf("%d rose\n", i);

      display.clear();
      display.display();
    }
    else if(b->fell()) {
      Serial.println("fell");
      display.clear();
      display.printf("%d Down", button_assignments[i].pin);
      display.display();
    }
    //Serial.printf("%d,%d  ", i, digitalRead(button_assignments[i].pin));
  }
}

const ButtonAssignment * const getButtonForPin(int pin) {
  if(pin < 0) {
    return NULL;
  }

  //TODO linear array search
  return NULL;
}
