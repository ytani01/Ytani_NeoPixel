/**
 *
 */
#include "common.h"

const int  HS_N = 7;
const hs_t HS[HS_N] =
  {
   {0x10000 *   0 / 360, 0x00}, // white
   {0x10000 *   0 / 360, 0xff}, // red
   {0x10000 *  60 / 360, 0xff}, // yellow
   {0x10000 * 120 / 360, 0xff}, // green
   {0x10000 * 180 / 360, 0xff}, // cyan
   {0x10000 * 240 / 360, 0xff}, // blue
   {0x10000 * 300 / 360, 0xff}  // purple
  };

void setHS(Ytani_NeoPixel *leds, int led_i, int hs_i, uint8_t br) {
  uint32_t col = leds->colorHSV(HS[hs_i].hue, HS[hs_i].sat, br);
  leds->setColor(led_i, col);
}  
