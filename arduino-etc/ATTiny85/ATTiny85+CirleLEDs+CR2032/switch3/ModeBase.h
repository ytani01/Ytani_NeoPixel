/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H

#include "Ytani_NeoPixel.h"
#include "Button.h"

class ModeBase {
 public:
  ModeBase(Ytani_NeoPixel *leds, Button *btn);

  virtual void loop();
  virtual boolean btn_loop_hdr();

 protected:
  Ytani_NeoPixel *_leds;
  Button         *_btn;  
};
#endif // MODE_BASE_H
