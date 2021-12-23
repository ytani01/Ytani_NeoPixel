/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_White.h"

/**
 *
 */
void Mode_White::display(Ytani_NeoPixel *leds) {
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    leds->setColorHSVdeg(led_i, this->_cur_hue_deg, 0x0, CurBr);
  } // for(led_i)
}
