/**
 *
 */
#include "Ytani_NeoPixel.h"

const uint8_t PIN_PIXEL        = 4;
const uint8_t PIXELS_N         = 1;
const uint8_t PIXELS_N_MAX     = 64;
const int     BRIGHTNESS       = 255;  // 1-255

const unsigned long LOOP_DELAY = 10;  // ms

Ytani_NeoPixel pixels(PIXELS_N_MAX, PIN_PIXEL);

/**
 *
 */
void setup() {
  pixels.clear();
  
  /*
  for (int i=0; i < PIXELS_N; i++) {
    pixels.setColor(i, 0x323232);
  } // for(i)
  */
  pixels.setColor(6, 0x323232);
  pixels.setColor(5, 0x000032);
  pixels.show();

} // setup()

/**
 *
 */
void loop() {
  /*
  pixels.setColor(6, 0x808080);
  pixels.show();
  delay(LOOP_DELAY);

  pixels.setColor(6, 0);
  pixels.show();
  */
  delay(LOOP_DELAY);
} // loop()
