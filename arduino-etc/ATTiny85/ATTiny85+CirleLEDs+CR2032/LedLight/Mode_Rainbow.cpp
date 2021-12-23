/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Rainbow.h"

/**
 *
 */
Mode_Rainbow::Mode_Rainbow(int eepCont): ModeBase() {
  if ( eepCont < 0 ) {
    return;
  }
  this->__eep_continuous = eepCont;
  EEPROM.get(this->__eep_continuous, this->_continuous);
  if ( this->_continuous > this->CONTINUOUS_INIT ) {
    this->_continuous = 0;
  }
}

/**
 *
 */
void Mode_Rainbow::loop(Ytani_NeoPixel *leds, Button *btn) {
  if ( this->_continuous > 0 ) {
    this->incHueDeg(this->DEG_INC);
    delay(this->_continuous);
  }
  this->display(leds);
}

/**
 *
 */
void Mode_Rainbow::display(Ytani_NeoPixel *leds) {
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    uint16_t hue_deg = (this->_cur_hue_deg + led_i * this->DEG_DIFF) % this->DEG_MAX;
    leds->setColorHSVdeg(led_i, hue_deg, 0xff, CurBr);
  } // for(led_i)
}

/**
 *
 */
boolean Mode_Rainbow::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
  static int repeat_count = 0;
  
  if ( btn->get_value() == Button::ON ) {
    if ( btn->is_repeated() ) {
      repeat_count++;
      if ( repeat_count >= this->REPEAT_UNIT ) {
        if ( this->_continuous == 0 ) {
          this->_continuous = this->CONTINUOUS_INIT;
        } else if ( repeat_count % this->REPEAT_UNIT == 0 ) {
          this->_continuous /= 2;
          if ( this->_continuous < 1 ) {
            this->_continuous = 1;
          }
        }
        EEPROM.put(this->__eep_continuous, this->_continuous);
      }
    }
    return true;
  }

  // Button::OFF
  int n = btn->get_click_count();
  repeat_count = 0;

  if ( n == 1 ) {
    if ( this->_continuous > 0 ) {
      this->_continuous = 0;
      EEPROM.put(this->__eep_continuous, this->_continuous);
    } else {
      this->_cur_hue_deg =
        ((this->_cur_hue_deg / this->DEG_DIFF) + 1) * this->DEG_DIFF;
    }
    return true;
  }
  
  return false;
} // loop_intr_hdr()

/**
 *
 */
void Mode_Rainbow::incHueDeg(uint16_t deg) {
  this->_cur_hue_deg = (this->_cur_hue_deg + deg) % this->DEG_MAX;
}
