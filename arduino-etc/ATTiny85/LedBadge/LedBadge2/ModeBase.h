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
  ModeBase(Ytani_NeoPixel *leds, Button *btn) {
    this->_leds = leds;
    this->_btn = btn;
  };

  virtual void loop() = 0;
  virtual boolean btn_loop_hdr() = 0;

protected:
  Ytani_NeoPixel *_leds;
  Button *_btn;
};

#endif // _MODE_BASE_H_
