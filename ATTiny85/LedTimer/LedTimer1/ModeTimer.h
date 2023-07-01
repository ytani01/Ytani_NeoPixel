/**
 * Copyright (c) 2022 Yoichi Tanibayashi
 */
#ifndef _MODE_TIMER_H_
#define _MODE_TIMER_H_

#include "ModeBase.h"

typedef unsigned long msec_t;

/**
 *
 */
class ModeTimer: public ModeBase {

public:
  static const msec_t TIME_LIMIT[] = {1, 2, 3, 4, 5,
                                      10, 20, 30, 40, 50,
                                      60, 120, 180, 240, 300};

  /**
   *
   */
  ModeTimer(Ytani_NeoPixel *leds, Button *btn,
            int eep_color_i=0, int eep_continuous=0)
    : ModeBase(leds, btn, eep_color_i, eep_continuous) {

  } // constructor

  /**
   *
   */
  virtual void display(hue_deg_t hue_deg, sat_t sat) {
    for (int i=0; i < LEDS_N; i++) {
      this->_leds->setColorHSVdeg(i, hue_deg, Ytani_NeoPixel::SAT_MAX,
                                  CurBr);

      hue_deg = (hue_deg - 60) % 360;
    } // for(i)
  } // display()

private:
  int _time_limit_i;
  msec_t _time_left;
  
}; // class ModeTimer
#endif // _MODE_TIMER_H_
