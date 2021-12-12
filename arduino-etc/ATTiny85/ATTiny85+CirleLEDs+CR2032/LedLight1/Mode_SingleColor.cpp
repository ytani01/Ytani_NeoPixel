/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
void Mode_SingleColor::display(Ytani_NeoPixel *leds) {
  for (int i=0; i < leds->pixels_n; i++) {
    leds->setColorHSV(i, HS[this->_cur_hs_i].hue, HS[this->_cur_hs_i].sat, this->_cur_br);
  } // for(i)

  leds->show();
  interrupts();
}
