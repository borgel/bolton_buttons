
#include "SmartKnob.h"
#include <Encoder.h>

bool SmartKnob::didChange() {
  if(this->knob.read() != this->lastPosition) {
    return true;
  }
  return false;
}

long SmartKnob::getChange() {
  // TODO return how much changed from last time THIS was called
  long prevKnob = this->lastPosition;
  this->lastPosition = this->knob.read();
  
  return this->lastPosition - prevKnob;
}

long SmartKnob::getPosition() {
  return this->lastPosition;
}
