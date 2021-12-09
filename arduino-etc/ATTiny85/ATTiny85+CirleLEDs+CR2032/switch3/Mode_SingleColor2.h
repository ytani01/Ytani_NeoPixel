/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_SINGLE_COLOR2_H
#define MODE_SINGLE_COLOR2_H

#include "ModeBase.h"


class Mode_SingleColor2: public ModeBase {
public:
  Mode_SingleColor2();

  void loop(Ytani_NeoPixel *leds, Button *btn);
  boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);

  void incHS();
  void setHS(Ytani_NeoPixel *leds, int i, int hs);

private:
  const uint8_t BRIGHTNESS_MAX = 64;
  uint8_t       CurBr = BRIGHTNESS_MAX;

  const unsigned long HS[7][2] =
    {
     {0x10000 *   0 / 360, 0x00}, // white
     {0x10000 *   0 / 360, 0xff}, // red
     {0x10000 *  60 / 360, 0xff}, // yellow
     {0x10000 * 120 / 360, 0xff}, // green
     {0x10000 * 180 / 360, 0xff}, // cyan
     {0x10000 * 240 / 360, 0xff}, // blue
     {0x10000 * 300 / 360, 0xff}  // purple
    };

  unsigned long CurHS = 0;
  unsigned long HS_N = sizeof(HS) / sizeof(HS[0]);
};
#endif // MODE_SINGLE_COLOR2_H
