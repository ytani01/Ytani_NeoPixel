/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_WHITE_H
#define MODE_WHITE_H

#include "Mode_Rainbow.h"

class Mode_White: public Mode_Rainbow {
public:
  Mode_White() {};
  void display(Ytani_NeoPixel *leds) override;
  boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);
};
#endif // MODE_WHITE_H
