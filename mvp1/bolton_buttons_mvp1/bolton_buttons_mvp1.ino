

#include <Adafruit_DotStar.h>
#include <Bounce2.h>
#include <SPI.h>
#include <Wire.h>

// not sure if this is needed
#define SSD1306_128_32

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <usb_keyboard.h>

#include "buttonmap.h"

//TODO package
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(-1);

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
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32?
  display.display();
  display.clearDisplay();
  display.drawPixel(10, 10, WHITE);
  display.drawPixel(10, 11, WHITE);
  display.drawPixel(10, 12, WHITE);
  display.display();

  /*
  strip.setPixelColor(0, 10, 100, 50);
  strip.setPixelColor(8, 200, 100, 50);
  strip.show();
  */

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
    }
    else if(b->fell()) {
      Serial.println("fell");
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
