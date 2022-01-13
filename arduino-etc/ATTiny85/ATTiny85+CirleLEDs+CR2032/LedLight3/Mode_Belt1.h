/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_BELT1_H
#define MODE_BELT1_H

#include "Mode_Rainbow.h"

class Mode_Belt1: public Mode_Rainbow {
public:
  Mode_Belt1(int eepCont, int eepSat): Mode_Rainbow(eepCont, eepSat) {};
  void display(Ytani_NeoPixel *leds, bool force) override;
};
#endif // MODE_BELT1_H
