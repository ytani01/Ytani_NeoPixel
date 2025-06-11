/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef _MODE_RAINBOW_H_
#define _MODE_RAINBOW_H_

#include "ModeBase.h"

class ModeRainbow: public ModeBase {
public:
  ModeRainbow(Ytani_NeoPixel *leds, Button *btn,
              int eep_color_i=0, int eep_continuous=0)
    : ModeBase(leds, btn, eep_color_i, eep_continuous) {
  } // constructor

  /**
   *
   */
  virtual void display(mode_base_color_t hue_deg) {
    for (int i=0; i < LEDS_N; i++) {
      this->_leds->setColorHSVdeg(i, hue_deg, 255, CurBr);
      hue_deg = (hue_deg + 60) % 360;
    }
  } // display()
}; // class ModeRainbow
#endif // _MODE_RAINBOW_H_
