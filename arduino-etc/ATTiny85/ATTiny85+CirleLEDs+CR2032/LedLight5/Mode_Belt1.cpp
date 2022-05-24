/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Belt1.h"

/**
 *
 */
void Mode_Belt1::display(Ytani_NeoPixel *leds, bool force=false) {
  uint16_t hue_deg = this->_cur_hue_deg;
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    this->display1(leds, led_i, hue_deg);
    if ( led_i % this->_belt_w == this->_belt_w - 1 ) {
      hue_deg = (hue_deg + this->DEG_DIFF * 3) % this->DEG_MAX;
      //hue_deg = (hue_deg + 180) % this->DEG_MAX;
    }
  } // for(led_i)
}
