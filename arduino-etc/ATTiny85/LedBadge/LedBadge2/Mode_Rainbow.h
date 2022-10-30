/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef _MODE_RAINBOW_H_
#define _MODE_RAINBOW_H_

#include "ModeBase.h"

class Mode_Rainbow: public ModeBase {
public:
  const uint16_t DEG_MAX = 360;
  const uint16_t DEG_DIFF = 60;
  const uint16_t DEG_INC = 2;

  const uint16_t CONTINUOUS_MAX = 500;
  const uint8_t CONTINUOUS_DEF = 64;
  const uint8_t REPEAT_UNIT = 1;

  Mode_Rainbow(Ytani_NeoPixel *leds, Button *btn, int eepCont, int eepSat);

  virtual void loop();
  virtual boolean btn_loop_hdr();

  virtual void display1(int led_n, uint16_t hue_deg);
  virtual void display(bool force=false);

  virtual void incHueDeg(uint16_t deg);

protected:
  uint16_t _cur_hue_deg = 0;
  uint8_t _cur_sat = 0xff;
  unsigned long _continuous = 0;
  int __eep_continuous, __eep_sat;
};
#endif // _MODE_RAINBOW_H_
