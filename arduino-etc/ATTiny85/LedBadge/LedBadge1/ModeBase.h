/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef _MODE_BASE_H_
#define _MODE_BASE_H_

#include <EEPROM.h>
#include "Ytani_NeoPixel.h"
#include "Button.h"

extern const uint8_t LEDS_N;
extern const uint8_t BRIGHTNESS_MAX;
extern uint8_t CurBr;

class ModeBase {
public:
  ModeBase() {};

  virtual void loop(Ytani_NeoPixel *leds, Button *btn) = 0;
  virtual boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) = 0;
};

#endif // _MODE_BASE_H_
