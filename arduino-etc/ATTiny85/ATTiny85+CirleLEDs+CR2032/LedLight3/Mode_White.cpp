/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_White.h"

/**
 *
 */
void Mode_White::display(Ytani_NeoPixel *leds) {
  for (int i=0; i < leds->pixels_n; i++) {
    leds->setColorHSVdeg(i, this->_cur_hue_deg, 0x0, this->_cur_br);
  } // for(i)

  leds->show();
  interrupts();
}

/**
 *
 */
boolean Mode_White::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
  static int repeat_count = 0;
  
  if ( btn->get_value() == Button::ON ) {
    if ( btn->is_repeated() ) {
      repeat_count++;
      if ( repeat_count >= this->REPEAT_UNIT ) {
        if ( this->_continus == 0 ) {
          this->_continus = this->CONTINUS_INIT;
          return true;
        } else if ( repeat_count % this->REPEAT_UNIT == 0 ) {
          this->_continus /= 2;
          if ( this->_continus < 1 ) {
            this->_continus = 1;
          }
          return true;
        }
      }
    }
    return true;
  }

  // Button::OFF
  int n = btn->get_click_count();
  repeat_count = 0;

  if ( n == 1 ) {
    if ( this->_continus > 0 ) {
      this->_continus = 0;
    } else {
      this->_cur_br /= 2;
      if ( this->_cur_br <= 0 ) {
        this->_cur_br = this->BRIGHTNESS_MAX;
      }
    }
    return true;
  }
  
  if ( n == 2 ) {
    this->_cur_br /= 2;
    if ( this->_cur_br <= 0 ) {
      this->_cur_br = this->BRIGHTNESS_MAX;
    }
    return true;
  }

  return false;
} // btn_loop_hdr()/**
