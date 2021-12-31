/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
void Mode_SingleColor::display(Ytani_NeoPixel *leds, bool force=false) {
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    this->display1(leds, led_i, this->_cur_hue_deg);
  } // for(led_i)
}
