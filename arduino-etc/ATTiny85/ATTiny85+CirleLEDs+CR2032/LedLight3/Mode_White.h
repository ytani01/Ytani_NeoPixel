/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_WHITE_H
#define MODE_WHITE_H

#include "Mode_Rainbow.h"

class Mode_White: public Mode_Rainbow {
public:
  void display(Ytani_NeoPixel *leds) override;
};
#endif // MODE_WHITE_H
