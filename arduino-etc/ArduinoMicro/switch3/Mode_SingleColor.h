/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_SINGLE_COLOR_H
#define MODE_SINGLE_COLOR_H

#include "common.h"
#include "ModeBase.h"

class Mode_SingleColor: public ModeBase {
public:
  const uint8_t BRIGHTNESS_MAX = 64;
  uint8_t       CurBr = BRIGHTNESS_MAX;

  // constructor
  Mode_SingleColor();

  // override virtual
  void loop(Ytani_NeoPixel *leds, Button *btn) override;
  boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) override;

  // local function
  void incHS();

private:
  int _cur_hs = 0;
};
#endif // MODE_SINGLE_COLOR_H
