/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_BELT1_H
#define MODE_BELT1_H

#include "Mode_Rainbow.h"

class Mode_Belt1: public Mode_Rainbow {
public:
  const uint8_t DEF_BELT_W = 6;
  
  Mode_Belt1(int eepCont, int eepSat): Mode_Rainbow(eepCont, eepSat) {};
  void display(Ytani_NeoPixel *leds, bool force) override;

protected:
  uint8_t _belt_w = DEF_BELT_W;
};
#endif // MODE_BELT1_H
