#pragma once

#include <Encoder.h>
#include <Arduino.h>

class SmartKnob 
{
public:
  SmartKnob(uint8_t pinA, uint8_t pinB): knob(Encoder(pinA, pinB)), lastPosition(0) {}

  bool didChange();
  long getChange();
  long getPosition();

private:
  // gets re-init later on construction of SmartKnob
  Encoder knob;

  long lastPosition;
};
