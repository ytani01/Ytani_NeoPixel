/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_RANDOM1_H
#define MODE_RANDOM1_H

#include "Mode_Rainbow.h"

class Mode_Random1: public Mode_Rainbow {
public:
  Mode_Random1(int eepCont): Mode_Rainbow(eepCont) {};
  void display(Ytani_NeoPixel *leds) override;
};
#endif // MODE_RANDOM1_H
