/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_RANDOM1_H
#define MODE_RANDOM1_H

#include "Mode_Rainbow.h"

class Mode_Random1: public Mode_Rainbow {
public:
  Mode_Random1(int eepCont, int eepSat): Mode_Rainbow(eepCont, eepSat) {};
  void display(Ytani_NeoPixel *leds, bool force) override;
};
#endif // MODE_RANDOM1_H
