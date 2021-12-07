/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_SINGLE_COLOR_H
#define MODE_SINGLE_COLOR_H

#include "ModeBase.h"

class Mode_SingleColor: public ModeBase {
public:
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

  const uint8_t BRIGHTNESS_MAX = 255;
  uint8_t       CurBr = BRIGHTNESS_MAX >> 4;

  Mode_SingleColor(Ytani_NeoPixel *leds, Button *btn);

  void loop();
  void btn_loop_hdr();

  void incHS();
  void setHS(int i, int hs);
};
#endif // MODE_SINGLE_COLOR_H
