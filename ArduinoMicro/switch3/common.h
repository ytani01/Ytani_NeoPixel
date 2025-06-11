#ifndef _COMMON_H
#define _COMMON_H

#include <Arduino.h>
#include "Ytani_NeoPixel.h"

typedef struct  {
  uint32_t hue;
  uint8_t  sat;
} hs_t;

extern const int HS_N;
extern const hs_t HS[];
extern void setHS(Ytani_NeoPixel *leds, int led_i, int hs_i, uint8_t br);

#endif // _COMMON_H
