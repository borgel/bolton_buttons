
#include "SmartKnob.h"
#include <Encoder.h>

bool SmartKnob::didChange() {
  long d = this->lastPosition - this->knob.read();
  // only count it as changed through the lens of the divisor
  if(d) {
    // if the change is in excess of one divisor
    if(abs(d) >= this->divisor){
      return true;
    }
  }
  return false;
}

long SmartKnob::getChange() {
  // return how much changed from last time THIS FXN was called
  long prevKnob = this->lastPosition;
  this->lastPosition = this->knob.read();
  
  return (this->lastPosition - prevKnob) / this->divisor;
}

long SmartKnob::getPosition() {
  return this->lastPosition;
}
