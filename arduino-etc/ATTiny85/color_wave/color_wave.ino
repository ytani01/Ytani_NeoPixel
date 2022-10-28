/**
 *
 */
#include "Ytani_NeoPixel.h"

const uint8_t PIN_PIXEL        = 4;
const uint8_t PIXELS_N         = 32;
const int     BRIGHTNESS       = 32;  // 1-255
const int     HUE_STEP         = 0x1800;

const unsigned long LOOP_DELAY = 100;  // ms

Ytani_NeoPixel pixels(PIXELS_N, PIN_PIXEL);

/**
 *
 */
void set_colorHSV_and_show(uint32_t hue0, uint32_t hue_step,
                           unsigned long delay_msec=0) {
  for (int i=0; i < PIXELS_N; i++) {
    uint32_t col = pixels.colorHSV(hue0, 255, BRIGHTNESS);
    pixels.setColor(i, col);

    hue0 -= HUE_STEP;

    if ( delay_msec > 0 ) {
      pixels.show();
      delay(delay_msec);
    }
  } // for(i)

  if (delay_msec == 0) {
    pixels.show();
  }
} // set_colorHSV_and_show()

/**
 *
 */
void setup() {
  pixels.clear();
  pixels.show();
} // setup()

/**
 *
 */
void loop() {
  static uint32_t hue = 0;

  set_colorHSV_and_show(hue, HUE_STEP);
  hue = (hue + HUE_STEP) % 0x10000;

  delay(LOOP_DELAY);
} // loop()
