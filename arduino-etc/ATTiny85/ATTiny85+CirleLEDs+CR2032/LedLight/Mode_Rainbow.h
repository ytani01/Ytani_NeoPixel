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

  const uint16_t CONTINUOUS_INIT = 500;
  const uint8_t CONTINUOUS_DEF = 64;
  const uint8_t REPEAT_UNIT = 1;

  Mode_Rainbow(int eepCont);

  virtual void loop(Ytani_NeoPixel *leds, Button *btn);
  virtual boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);
  virtual void display(Ytani_NeoPixel *leds);

  virtual void incHueDeg(uint16_t deg);

protected:
  uint16_t _cur_hue_deg = 0;
  unsigned long _continuous = 0;
  int __eep_continuous;
};
#endif // MODE_RAINBOW_H
