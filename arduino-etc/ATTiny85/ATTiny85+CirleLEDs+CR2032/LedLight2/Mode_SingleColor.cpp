/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
void Mode_SingleColor::display(Ytani_NeoPixel *leds) {
  for (int i=0; i < leds->pixels_n; i++) {
    leds->setColorHSVdeg(i, this->_cur_hue_deg, 0xff, this->_cur_br);
  } // for(i)

  leds->show();
  interrupts();
}
