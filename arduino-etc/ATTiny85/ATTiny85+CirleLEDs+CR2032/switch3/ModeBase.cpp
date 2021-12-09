/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "ModeBase.h"

ModeBase::ModeBase(Ytani_NeoPixel *leds, Button *btn) {
  this->_leds = leds;
  this->_btn  = btn;
} // ModeBase()

void ModeBase::loop() {
}

boolean ModeBase::btn_loop_hdr() {
}
