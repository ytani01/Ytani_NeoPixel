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
    this->setHS(leds, i, this->CurHS);
  } // for(i)
  leds->show();
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
void Mode_SingleColor::incHS() {
  this->CurHS++;
  if ( this->CurHS >= this->HS_N ) {
    this->CurHS = 0;
  }
} // incHS()

/**
 *
 */
void Mode_SingleColor::setHS(Ytani_NeoPixel *leds, int i, int hs) {
  uint32_t col = leds->colorHSV(this->HS[hs][0], this->HS[hs][1], this->CurBr);
  leds->setColor(i, col);
}
