/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_RAINBOW_H
#define MODE_RAINBOW_H

#include "common.h"
#include "ModeBase.h"

class Mode_Rainbow: public ModeBase {
public:
  const uint8_t BRIGHTNESS_MAX = 64;

  Mode_Rainbow();

  void loop(Ytani_NeoPixel *leds, Button *btn);
  boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);
  virtual void display(Ytani_NeoPixel *leds);

  void incHS();

protected:
  int _cur_hs_i = 0;
  uint8_t _cur_br = BRIGHTNESS_MAX >> 2;
  unsigned long _continus = 0;
};
#endif // MODE_RAINBOW_H
