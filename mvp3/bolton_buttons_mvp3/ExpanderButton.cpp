#include "ExpanderButton.h"
#include "TCA9555.h"
#include "buttonmap.h"
#include <i2c_t3.h>

//TODO C++ify this

static TCA9555 tca9555 = TCA9555(1,0,0);

static ep_ButtonChangeCB changeCB;

static bool buttonsChanged;
static uint16_t lastButtonMask;
static uint16_t currentButtonMask;
static void buttonChangeISR();

void ep_Init(uint8_t const interruptPin, ep_ButtonChangeCB const cb) {
  changeCB = cb;
  
  // IO expander interrupt port
  pinMode(interruptPin, INPUT);
  // fires on every press, and every release
  attachInterrupt(interruptPin, buttonChangeISR, FALLING);
  
  // setup IO expander
  // each port all inputs, except last bit of second
  // port 0. bit set to 1 is input
  tca9555.setPortDirection(0, 0xFF);
  tca9555.setPortDirection(1, (byte)(~(1 << 7)));

  // grab initial states
  lastButtonMask = 0x7FFF & tca9555.getInputStates();
  currentButtonMask = lastButtonMask;
  buttonsChanged = false;
}

void ep_HandleChanges() {
  if(buttonsChanged) {
    // consider everything handled
    buttonsChanged = false;

    // find changes between button states
    uint16_t current = currentButtonMask;
    uint16_t change = lastButtonMask ^ currentButtonMask;
    
    // there are NUM_INDIRECT_BUTTONS buttons connected, starting at pin 0
    for(int i = 0; i < NUM_INDIRECT_BUTTONS; i++) {
      if(change & 0x1) {
        // if 1, was a raise
        changeCB(i, !(current & 0x1));
      }
      change >>= 1;
      current >>= 1;
    }
  }
}

// fires when the IO expander detects a change
static void buttonChangeISR() {
  lastButtonMask = currentButtonMask;
  currentButtonMask = 0x7FFF & tca9555.getInputStates();
  buttonsChanged = true;
}
