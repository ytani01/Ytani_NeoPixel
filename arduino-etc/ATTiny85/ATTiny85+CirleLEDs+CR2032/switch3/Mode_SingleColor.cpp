/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_SingleColor.h"

/**
 *
 */
Mode_SingleColor::Mode_SingleColor(Ytani_NeoPixel *leds, Button *btn):
  ModeBase::ModeBase(leds, btn) {
}

/**
 *
 */
void Mode_SingleColor::loop() {
  for (int i=0; i < this->_leds->pixels_n; i++) {
    this->setHS(i, CurHS);
  } // for(i)
  this->_leds->show();
}

/**
 *
 */
boolean Mode_SingleColor::btn_loop_hdr() {
  if ( this->_btn->get_value() == Button::ON ) {
    if ( this->_btn->is_repeated() ) {
      this->incHS();
    }
    return true;
  }

  // Button::OFF
  int n = this->_btn->get_click_count();

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
void Mode_SingleColor::setHS(int i, int hs) {
  uint32_t col = this->_leds->colorHSV(HS[hs][0], HS[hs][1], CurBr);
  this->_leds->setColor(i, col);
}
