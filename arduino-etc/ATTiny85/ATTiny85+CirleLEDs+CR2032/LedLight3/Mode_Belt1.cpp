/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Belt1.h"

/**
 *
 */
void Mode_Belt1::display(Ytani_NeoPixel *leds, bool force=false) {
  if ( this->_continuous == 0 ) {
    this->_continuous = this->CONTINUOUS_INIT;
  }
  leds->setColorHSVdeg(belt(LEDS_N), random(DEG_MAX), 0xff, CurBr);
}
