#pragma once

#include <stdint.h>

// 9 keys, rotary, bonus switch, stop element
#define NUM_BUTTONS (9+1+1)

typedef struct {
  int pin;
  //TODO type?
  uint32_t key;
} ButtonAssignment;

// TODO separate pin from key
ButtonAssignment button_assignments[NUM_BUTTONS] = {
  {2, KEY_W},   //f
  {3, KEY_W},   //f
  {4, KEY_W},   //
  {5, KEY_W},   //
  {6, KEY_W},   //
  {7, KEY_W},   //
  {8, KEY_W},   //
  {9, KEY_W},   //
  {10, KEY_W},  //
  
  {16, KEY_W},  // bonus buttons
  {15, KEY_W},  // encoder switch
};
