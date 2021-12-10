/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
void Mode_SingleColor::display(Ytani_NeoPixel *leds) {
  for (int i=0; i < leds->pixels_n; i++) {
    setHS(leds, i, this->_cur_hs, CurBr);
  } // for(i)

  leds->show();
  interrupts();
}
