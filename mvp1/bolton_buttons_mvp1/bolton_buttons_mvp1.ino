
#include <usb_keyboard.h>

#include "buttonmap.h"

void setup() {
  // wait for USB enumeration
  delay(1000);
  Serial.begin(9600);
  Serial.println("Booted...");
  
  // attach bouncer to all buttons
  ButtonAssignment *ba = &button_assignments[0];
  for(int i = 0; button_assignments[i].pin != BUTTON_ASSIGNMENT_UNASSIGNED; i++) {
    ba = &button_assignments[i];

    pinMode(ba->pin, INPUT);
    
    Serial.printf("Attached button on pin %d\n", ba->pin);
  }

  //TODO interrupt config

  Keyboard.print("a");

  //FIXME rm
  Serial.println("Done with setup");
}

void loop() {
  //check for key flags, and send data
}

const ButtonAssignment * const getButtonForPin(int pin) {
  if(pin == BUTTON_ASSIGNMENT_UNASSIGNED) {
    return NULL;
  }

  //TODO linear array search
  return NULL;
}
