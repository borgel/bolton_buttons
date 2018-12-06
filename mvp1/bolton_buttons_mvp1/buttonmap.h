#pragma once

#include <stdint.h>

// 9 keys, rotary, bonus switch, stop element
#define NUM_BUTTONS (9+1+1)

typedef struct {
  int pin;
  //TODO type?
  uint32_t key;
  int led_index;
} ButtonAssignment;

// TODO separate pin from key
ButtonAssignment button_assignments[NUM_BUTTONS] = {
  //pin, key to press and release on down
  {2, KEY_5, 7},   //5
  {3, KEY_7, 0},   //7
  {4, KEY_6, 6},   //6
  {5, KEY_2, 5},   //2
  {6, KEY_ESC, 4}, //esc
  {7, KEY_1, 8},   //1
  {8, KEY_4, 2},   //4
  {9, KEY_8, 1},   //8
  {10, KEY_3,3},   //3
  
  {16, KEY_W},  // bonus buttons
  {15, KEY_W},  // encoder switch
};
