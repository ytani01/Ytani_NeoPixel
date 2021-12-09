/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "ModeBase.h"

ModeBase::ModeBase() {
  this->_leds = leds;
  this->_btn  = btn;
} // ModeBase()

void ModeBase::loop(Ytani_NeoPixel *leds, Button *btn) {
}

boolean ModeBase::btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn) {
}
