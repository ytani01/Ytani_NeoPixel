/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Rainbow.h"

/**
 *
 */
Mode_Rainbow::Mode_Rainbow(int eepCont, int eepSat): ModeBase() {
  if ( eepCont < 0 ) {
    return;
  }

  this->__eep_continuous = eepCont;
  EEPROM.get(this->__eep_continuous, this->_continuous);
  if ( this->_continuous > this->CONTINUOUS_INIT ) {
    this->_continuous = 0;
  }

  this->__eep_sat = eepSat;
  EEPROM.get(this->__eep_sat, this->_cur_sat);
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
void Mode_Rainbow::display1(Ytani_NeoPixel *leds, int led_i, uint16_t hue_deg) {
  leds->setColorHSVdeg(led_i, hue_deg, this->_cur_sat, CurBr);
}

/**
 *
 */
void Mode_Rainbow::display(Ytani_NeoPixel *leds, bool force=false) {
  for (int led_i=0; led_i < leds->pixels_n; led_i++) {
    uint16_t hue_deg = (this->_cur_hue_deg + led_i * this->DEG_DIFF) % this->DEG_MAX;
    this->display1(leds, led_i, hue_deg);
    //leds->setColorHSVdeg(led_i, hue_deg, 0xff, CurBr);
  } // for(led_i)
}

/**
 *
 */
boolean Mode_Rainbow::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
  static int repeat_count = 0;
  int click_n = btn->get_click_count();
  
  if ( btn->get_value() == Button::ON ) {
    if ( btn->is_repeated() ) {
      this->_cur_sat = 0xff;
      EEPROM.put(this->__eep_sat, this->_cur_sat);

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
  repeat_count = 0;

  if ( click_n == 1 ) {
    if ( this->_continuous > 0 ) {
      this->_continuous = 0;
      EEPROM.put(this->__eep_continuous, this->_continuous);
    } else {
      if ( this->_cur_sat == 0 ) {
        this->_cur_sat = 0xff;
      } else {
        this->_cur_hue_deg =
          ((this->_cur_hue_deg / this->DEG_DIFF) + 1) * this->DEG_DIFF % DEG_MAX;
        if ( this->_cur_hue_deg == 0 ) {
          this->_cur_sat = 0;
        }
      }
      EEPROM.put(this->__eep_sat, this->_cur_sat);
      this->display(leds, true);
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
