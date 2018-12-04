#pragma once

#include <stdint.h>

#define BUTTON_ASSIGNMENT_UNASSIGNED -1
typedef struct {
  int pin;
  //TODO type?
  uint32_t key;
} ButtonAssignment;

// TODO separate pin from key
ButtonAssignment button_assignments[] = {
  {2, KEY_W},
  {15, KEY_W},  // encoder switch
  {BUTTON_ASSIGNMENT_UNASSIGNED},
};
