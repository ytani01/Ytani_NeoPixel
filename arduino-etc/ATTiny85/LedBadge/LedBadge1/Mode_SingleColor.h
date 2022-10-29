/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef _MODE_SINGLE_COLOR_H_
#define _MODE_SINGLE_COLOR_H_

#include "Mode_Rainbow.h"

class Mode_SingleColor: public Mode_Rainbow {
public:
  Mode_SingleColor(int eepCont, int eepSat): Mode_Rainbow(eepCont, eepSat) {};
  void display(Ytani_NeoPixel *leds, bool force) override;
};
#endif // _MODE_SINGLE_COLOR_H_
