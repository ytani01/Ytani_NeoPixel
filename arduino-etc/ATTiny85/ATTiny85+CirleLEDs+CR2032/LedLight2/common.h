#ifndef _COMMON_H
#define _COMMON_H

#include <Arduino.h>
#include "Ytani_NeoPixel.h"

#define _ATTINY85
//#define _ARDUINO

typedef struct  {
  uint16_t hue;
  uint8_t  sat;
} hs_t;

extern const int HS_N;
extern const hs_t HS[];

#endif // _COMMON_H
