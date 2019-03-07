
#include "SmartKnob.h"
#include <Encoder.h>

bool SmartKnob::didChange() {
  long prevKnob = this->lastPosition;
  this->lastPosition = this->knob.read();
  
  if(prevKnob != lastPosition) {
    return true;
  }
  return false;
}


long SmartKnob::getPosition() {
  return this->lastPosition;
}
