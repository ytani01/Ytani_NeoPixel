/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef _MODE_BASE_H_
#define _MODE_BASE_H_

#include <EEPROM.h>
#include "Ytani_NeoPixel.h"
#include "Button.h"

extern const uint8_t LEDS_N;
extern const uint8_t BRIGHTNESS_MAX;
extern uint8_t CurBr;

typedef uint16_t mode_base_color_t;

class ModeBase {
public:
  static const unsigned long CONTINUOUS_MSEC_MAX = 1024;
  static const mode_base_color_t CONTINUOUS_HUE_DEG_DIFF = 1;

  /**
   * @param leds NeoPixel
   * @param btn Button
   */
  ModeBase(Ytani_NeoPixel *leds, Button *btn,
           int eep_color_i=0, int eep_continuous=0) {
    this->_leds = leds;
    this->_btn = btn;
    this->_eep_color_i = eep_color_i;
    this->_eep_continuous = eep_continuous;
    
    // 不確定長な配列の初期化はこうするしかない?
    static mode_base_color_t p[] = {0, 60, 120, 180, 240, 300};
    this->_hue_deg = p;
    this->_color_n = sizeof(p) / sizeof(p[0]);

    this->_color_i = 0;
    if ( this->_eep_color_i != 0 ) {
      EEPROM.get(this->_eep_color_i, this->_color_i);
      if ( this->_color_i >= this->_color_n ) {
        this->_color_i = 0;
      }
    }

    this->_continuous = 0;
    if ( this->_eep_continuous != 0 ) {
      EEPROM.get(this->_eep_continuous, this->_continuous);
      if ( this->_continuous >= this->CONTINUOUS_MSEC_MAX ) {
        this->_continuous = 0;
      }
    }

    this->_continuous_hue_deg = 0;
  } // constructor

  /**
   * @param hue_deg 0..359
   */
  virtual void display(mode_base_color_t hue_deg) {
    this->_leds->clear();
    this->_leds->setColorHSVdeg(0, hue_deg, 255, CurBr);
#if 0
    for (int i=0; i < LEDS_N; i++) {
      this->_leds->setColorHSVdeg(i, hue_deg, 255, CurBr);
    }
#endif
  } // display()

  /**
   * this->_leds->show() は不要 (
   */
  virtual void loop() {
    unsigned long cur_msec = millis();
    static unsigned long prev_msec = 0;

    static mode_base_color_t hue_deg;

    if ( this->_continuous > 0 ) {
      if ( cur_msec - prev_msec > this->_continuous ) {
        hue_deg = (hue_deg + CONTINUOUS_HUE_DEG_DIFF) % 360;
        prev_msec = cur_msec;
      }
    } else {
      hue_deg = this->_hue_deg[this->_color_i];
    }

    this->display(hue_deg);
  } // loop()
  
  virtual boolean btn_loop_hdr() {
    int click_n = this->_btn->get_click_count();

    if ( this->_btn->get_value() == Button::ON ) {
      if ( this->_btn->is_long_pressed() ) {
        if ( this->_continuous == 0 ) {
          this->_continuous = this->CONTINUOUS_MSEC_MAX;
        } else {
          this->_continuous /= 2;
          if ( this->_continuous < 1 ) {
            this->_continuous = 1;
          }
        }
        EEPROM.put(this->_eep_continuous, this->_continuous);
      }
      return true;
    }

    // Button::OFF
    if ( click_n == 1 ) {
      if ( this->_continuous > 0 ) {
        this->_continuous = 0;
        EEPROM.put(this->_eep_continuous, this->_continuous);
      } else {
        this->_color_i = (this->_color_i + 1) % this->_color_n;
        EEPROM.put(this->_eep_color_i, this->_color_i);
      }
      return true;
    }

    return false;
  } // btn_loop_hdr()

protected:
  Ytani_NeoPixel *_leds;
  Button *_btn;
  int _eep_color_i, _eep_continuous;
  
  mode_base_color_t *_hue_deg;
  uint32_t _color_n;
  uint32_t _color_i;

  uint32_t _continuous;  
  mode_base_color_t *_continuous_hue_deg;
};

#endif // _MODE_BASE_H_
