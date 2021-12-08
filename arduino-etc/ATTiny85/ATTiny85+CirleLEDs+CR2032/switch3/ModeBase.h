/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H

#include "Ytani_NeoPixel.h"
#include "Button.h"

class ModeBase {
public:
  ModeBase() {};

  virtual void loop(Ytani_NeoPixel *leds, Button *btn) {return;};
  virtual boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {return false;};
};

#endif // MODE_BASE_H
