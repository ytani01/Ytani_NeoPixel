/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Random1.h"

/**
 *
 */
void Mode_Random1::display(Ytani_NeoPixel *leds, bool force=false) {
  if ( ! force && this->_continuous == 0 ) {
    return;
  }
  leds->setColorHSVdeg(random(LEDS_N), random(DEG_MAX), 0xff, CurBr);
}
