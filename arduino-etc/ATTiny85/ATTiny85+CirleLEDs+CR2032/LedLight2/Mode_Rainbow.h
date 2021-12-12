/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_RAINBOW_H
#define MODE_RAINBOW_H

#include "common.h"
#include "ModeBase.h"

class Mode_Rainbow: public ModeBase {
public:
  const uint16_t DEG_MAX = 360;
  const uint16_t DEG_DEF = 256;
  const uint16_t DEG_DIFF = 45;
  const uint8_t BRIGHTNESS_MAX = 64;

  const uint16_t CONTINUS_INIT = 256;
  const uint8_t CONTINUS_DEF = 64;
  const uint8_t REPEAT_UNIT = 3;

  Mode_Rainbow();

  void loop(Ytani_NeoPixel *leds, Button *btn);
  boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);
  virtual void display(Ytani_NeoPixel *leds);

  void incHueDeg(uint16_t deg=5);

protected:
  uint16_t _cur_hue_deg = 0;
  uint8_t _cur_br = BRIGHTNESS_MAX >> 2;
  unsigned long _continus = 0;
};
#endif // MODE_RAINBOW_H
