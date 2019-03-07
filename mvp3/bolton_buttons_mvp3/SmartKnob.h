#pragma once

#include <Encoder.h>
#include <Arduino.h>

class SmartKnob 
{
public:
  SmartKnob(uint8_t pinA, uint8_t pinB, uint8_t divisor = 4): knob(Encoder(pinA, pinB)), divisor(divisor), lastPosition(0) {}

  bool didChange();
  long getChange();
  long getPosition();

private:
  // gets re-init later on construction of SmartKnob
  Encoder knob;

  uint8_t divisor;
  long lastPosition;
};
