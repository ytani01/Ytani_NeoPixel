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
  if ( this->_continus > 0 ) {
    this->incHS();
    delay(this->_continus);
  }
  this->display(leds);
}

/**
 *
 */
void Mode_Rainbow::display(Ytani_NeoPixel *leds) {
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    int hs_i = (this->_cur_hs_i + led_i) % HS_N;
    leds->setColorHSV(led_i, HS[hs_i].hue, HS[hs_i].sat, this->_cur_br);
  } // for(led_i)

  leds->show();
  interrupts();
}

/**
 *
 */
boolean Mode_Rainbow::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
  static int repeat_count = 0;
  
  if ( btn->get_value() == Button::ON ) {
    if ( btn->is_repeated() ) {
      repeat_count++;
      if ( repeat_count < 5 ) {
        this->incHS();
      } else {
        if ( this->_continus == 0 ) {
          this->_continus = 256;
        } else if ( repeat_count % 5 == 0 ) {
          this->_continus /= 2;
          if ( this->_continus < 4 ) {
            this->_continus = 4;
          }
        }
      }
    }
    return true;
  }

  // Button::OFF
  int n = btn->get_click_count();
  repeat_count = 0;

  if ( n == 1 ) {
    this->_continus = 0;
    this->incHS();
    return true;
  }
  
  if ( n == 2 ) {
    this->_cur_br = this->_cur_br >> 2;
    if ( this->_cur_br <= 0 ) {
      this->_cur_br = this->BRIGHTNESS_MAX;
    }
    return true;
  }

  return false;
} // loop_intr_hdr()

/**
 *
 */
void Mode_Rainbow::incHS() {
  this->_cur_hs_i++;
  if ( this->_cur_hs_i >= HS_N ) {
    this->_cur_hs_i = 0;
  }
} // incHS()
