/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
Mode_SingleColor::Mode_SingleColor(): ModeBase::ModeBase() {
}

/**
 *
 */
void Mode_SingleColor::loop(Ytani_NeoPixel *leds, Button *btn) {
  for (int i=0; i < leds->pixels_n; i++) {
    this->setHS(leds, i, CurHS);
  } // for(i)
  this->leds->show();
}

/**
 *
 */
boolean Mode_SingleColor::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
  if ( btn->get_value() == Button::ON ) {
    if ( btn->is_repeated() ) {
      this->incHS();
    }
    return true;
  }

  // Button::OFF
  int n = btn->get_click_count();

  if ( n == 1 ) {
    this->incHS();
    return true;
  }
  
  if ( n == 2 ) {
    CurBr = CurBr >> 2;
    if ( CurBr <= 0 ) {
      CurBr = BRIGHTNESS_MAX;
    }
    return true;
  }

  return false;
} // loop_intr_hdr()

/**
 *
 */
void Mode_SingleColor::incHS() {
  CurHS++;
  if ( CurHS >= HS_N ) {
    CurHS = 0;
  }
} // incHS()

/**
 *
 */
void Mode_SingleColor::setHS(Ytani_NeoPixel *leds, int i, int hs) {
  uint32_t col = leds->colorHSV(HS[hs][0], HS[hs][1], CurBr);
  leds->setColor(i, col);
}
