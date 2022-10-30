/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Mode_Rainbow.h"

/**
 *
 */
Mode_Rainbow::Mode_Rainbow(Ytani_NeoPixel *leds, Button *btn,
                           int eepCont, int eepSat): ModeBase(leds, btn) {
  if ( eepCont < 0 ) {
    return;
  }

  this->__eep_continuous = eepCont;
  EEPROM.get(this->__eep_continuous, this->_continuous);
  if ( this->_continuous > this->CONTINUOUS_MAX ) {
    this->_continuous = 0;
    EEPROM.put(this->__eep_continuous, this->_continuous);
  }

  this->__eep_sat = eepSat;
  EEPROM.get(this->__eep_sat, this->_cur_sat);
}

/**
 *
 */
void Mode_Rainbow::loop() {
  if ( this->_continuous > 0 ) {
    this->incHueDeg(this->DEG_INC);
    delay(this->_continuous);
  }
  this->display(this->_leds);
}

/**
 *
 */
void Mode_Rainbow::display1(int led_i, uint16_t hue_deg) {
  this->_leds->setColorHSVdeg(led_i, hue_deg, this->_cur_sat, CurBr);
}

/**
 *
 */
void Mode_Rainbow::display(bool force) {
  for (int led_i=0; led_i < this->_leds->pixels_n; led_i++) {
    uint16_t hue_deg = (this->_cur_hue_deg + led_i * this->DEG_DIFF) % this->DEG_MAX;
    this->display1(led_i, hue_deg);
    //leds->setColorHSVdeg(led_i, hue_deg, 0xff, CurBr);
  } // for(led_i)
}

/**
 *
 */
boolean Mode_Rainbow::btn_loop_hdr() {
  static int repeat_count = 0;
  int click_n = this->_btn->get_click_count();
  
  if ( this->_btn->get_value() == Button::ON ) {
    if ( this->_btn->is_repeated() ) {
      this->_cur_sat = 0xff;
      EEPROM.put(this->__eep_sat, this->_cur_sat);

      repeat_count++;
      if ( repeat_count < this->REPEAT_UNIT ) {
        return true;
      }

      if ( this->_continuous == 0 ) {
        // 連続変化開始
        this->_continuous = this->CONTINUOUS_MAX;
      } else if ( repeat_count % this->REPEAT_UNIT == 0 ) {
        // 変化間隔短縮
        this->_continuous /= 2;
        if ( this->_continuous < 1 ) {
          this->_continuous = 1;
        }
      }
      EEPROM.put(this->__eep_continuous, this->_continuous);
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
      this->display(true);
    }
    return true;
  }
  
  return false;
} // loop_loop_hdr()

/**
 *
 */
void Mode_Rainbow::incHueDeg(uint16_t deg) {
  this->_cur_hue_deg = (this->_cur_hue_deg + deg) % this->DEG_MAX;
}
