/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Rainbow.h"

/**
 *
 */
Mode_Rainbow::Mode_Rainbow(): ModeBase() {
}

/**
 *
 */
void Mode_Rainbow::loop(Ytani_NeoPixel *leds, Button *btn) {
  for (int i=0; i < leds->pixels_n; i++) {
    setHS(leds, i, this->_cur_hs, CurBr);
  } // for(i)
  leds->show();
  interrupts();
}

/**
 *
 */
boolean Mode_Rainbow::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
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
    this->CurBr = this->CurBr >> 2;
    if ( this->CurBr <= 0 ) {
      this->CurBr = this->BRIGHTNESS_MAX;
    }
    return true;
  }

  return false;
} // loop_intr_hdr()

/**
 *
 */
void Mode_Rainbow::incHS() {
  this->_cur_hs++;
  if ( this->_cur_hs >= HS_N ) {
    this->_cur_hs = 0;
  }
} // incHS()
