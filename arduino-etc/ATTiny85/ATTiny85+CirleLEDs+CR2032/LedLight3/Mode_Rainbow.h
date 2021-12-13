/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_RAINBOW_H
#define MODE_RAINBOW_H

#include "ModeBase.h"

class Mode_Rainbow: public ModeBase {
public:
  const uint16_t DEG_MAX = 360;
  const uint16_t DEG_DIFF = 45;
  const uint16_t DEG_INC = 2;
  const uint8_t BRIGHTNESS_MAX = 64;

  const uint16_t CONTINUS_INIT = 256;
  const uint8_t CONTINUS_DEF = 64;
  const uint8_t REPEAT_UNIT = 1;

  Mode_Rainbow();

  virtual void loop(Ytani_NeoPixel *leds, Button *btn);
  virtual boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);
  virtual void display(Ytani_NeoPixel *leds);

  virtual void incHueDeg(uint16_t deg);

protected:
  uint16_t _cur_hue_deg = 0;
  uint8_t _cur_br = BRIGHTNESS_MAX >> 2;
  unsigned long _continus = 0;
};
#endif // MODE_RAINBOW_H
