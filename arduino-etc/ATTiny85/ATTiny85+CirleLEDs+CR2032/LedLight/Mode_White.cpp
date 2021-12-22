/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_White.h"

/**
 *
 */
void Mode_White::display(Ytani_NeoPixel *leds) {
  for (int i=0; i < leds->pixels_n; i++) {
    leds->setColorHSVdeg(i, this->_cur_hue_deg, 0x0, CurBr);
  } // for(i)

  leds->show();
  interrupts();
}
