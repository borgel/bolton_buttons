#pragma once

#include "TCA9555.h"
#include <Arduino.h>

typedef void (*ep_ButtonChangeCB)(uint8_t const pin, bool const wasPress);

void ep_Init(uint8_t const interruptPin, ep_ButtonChangeCB const cb);
void ep_HandleChanges();
