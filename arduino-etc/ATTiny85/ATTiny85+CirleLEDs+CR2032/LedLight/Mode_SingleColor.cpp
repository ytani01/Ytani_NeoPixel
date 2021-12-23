/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
void Mode_SingleColor::display(Ytani_NeoPixel *leds) {
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    leds->setColorHSVdeg(led_i, this->_cur_hue_deg, 0xff, CurBr);
  } // for(led_i)
}
