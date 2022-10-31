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

class ModeBase {
public:
  /**
   * @param leds NeoPixel
   * @param btn Button
   */
  ModeBase(Ytani_NeoPixel *leds, Button *btn) {
    this->_leds = leds;
    this->_btn = btn;
  };

  /**
   * this->_leds->show() は不要 (
   */
  virtual void loop() {
    this->_leds->setBrightness(CurBr);
    this->_leds->setColor(0, 0xffffff);
    this->_leds->setColor(1, 0x0000ff);
    this->_leds->setColor(2, 0xff0000);
    this->_leds->setColorHSVdeg(3, 240, 0xff, 0xff);
    this->_leds->setColorHSVdeg(4, 0, 0xff, 0xff);
  };
  
  virtual boolean btn_loop_hdr() {
    return false;
  };

protected:
  Ytani_NeoPixel *_leds;
  Button *_btn;
};

#endif // _MODE_BASE_H_
