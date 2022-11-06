/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef _MODE_CROSS_H_
#define _MODE_CROSS_H_

#include "ModeBase.h"

class ModeCross: public ModeBase {
public:
  ModeCross(Ytani_NeoPixel *leds, Button *btn,
              int eep_color_i=0, int eep_continuous=0)
    : ModeBase(leds, btn, eep_color_i, eep_continuous) {
  } // constructor

  /**
   *
   */
  virtual void display(hue_t hue_deg) {
    this->_leds->setColorHSVdeg(0, hue_deg, 255, CurBr);
    hue_deg = (hue_deg + 120) % 360;
    this->_leds->setColorHSVdeg(1, hue_deg, 255, CurBr);
    this->_leds->setColorHSVdeg(3, hue_deg, 255, CurBr);
    hue_deg = (hue_deg + 120) % 360;
    this->_leds->setColorHSVdeg(2, hue_deg, 255, CurBr);
    this->_leds->setColorHSVdeg(4, hue_deg, 255, CurBr);
  } // display()
}; // class ModeCross
#endif // _MODE_CROSS_H_
