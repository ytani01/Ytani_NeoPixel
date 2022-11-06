/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#ifndef _MODE_BASE_H_
#define _MODE_BASE_H_

#include <EEPROM.h>
#include "Ytani_NeoPixel.h"
#include "Button.h"

extern const uint8_t LEDS_N;
extern const br_t BRIGHTNESS_MAX;
extern br_t CurBr;

static constexpr hue_t HUE_DEG[] = {0, 15, 50, 120, 180, 240, 300};
static const size_t HUE_DEG_N = sizeof(HUE_DEG) / sizeof(HUE_DEG[0]);

class ModeBase {
public:
  static const unsigned long CONTINUOUS_MSEC_MAX = 1024;
  static const hue_t CONTINUOUS_HUE_DEG_DIFF = 1;

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
    
    this->_color_i = 0;
    if ( this->_eep_color_i != 0 ) {
      EEPROM.get(this->_eep_color_i, this->_color_i);
      if ( this->_color_i >= HUE_DEG_N ) {
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
   * @param hue_deg 0..255
   */
  virtual void display(hue_t hue_deg) {
    this->_leds->clear();
    this->_leds->setColorHSVdeg(0, hue_deg, Ytani_NeoPixel::SAT_MAX, CurBr);
  } // display()

  /**
   * this->_leds->show() は不要 (
   */
  virtual void loop() {
    unsigned long cur_msec = millis();
    static unsigned long prev_msec = 0;

    static hue_t hue_deg;

    if ( this->_continuous > 0 ) {
      if ( cur_msec - prev_msec > this->_continuous ) {
        hue_deg = (hue_deg + CONTINUOUS_HUE_DEG_DIFF) % 360;
        prev_msec = cur_msec;
      }
    } else {
      hue_deg = HUE_DEG[this->_color_i];
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
        this->_color_i = (this->_color_i + 1) % HUE_DEG_N;
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
  
  uint32_t _color_i;

  uint32_t _continuous;  
  hue_t *_continuous_hue_deg;
};

#endif // _MODE_BASE_H_
