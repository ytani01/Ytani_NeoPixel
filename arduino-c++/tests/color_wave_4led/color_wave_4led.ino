#include "Ytani_NeoPixel.h"

//const uint8_t PIN_ONBOARD_LED = 1;  // DigiSpark ATTiny85
//const uint8_t PIN_ONBOARD_LED = 13;  // Arduino Uno

const uint8_t PIN_PIXEL        = 4;
const uint8_t PIN_GND          = 3;
const uint8_t PIXELS_N         = 32;
const int     BRIGHTNESS       = 64;  // 1-255
const int     HUE_STEP         = 0x1800;

const unsigned long LOOP_DELAY = 100;  // ms

const uint8_t COLOR_LIST[][3] =
  {
   {BRIGHTNESS, 0, 0},
   {BRIGHTNESS, int(1*BRIGHTNESS/6), 0},
   {BRIGHTNESS, BRIGHTNESS, 0},
   {int(3*BRIGHTNESS/8), BRIGHTNESS, 0},
   {0, BRIGHTNESS, 0},
   {0, BRIGHTNESS, int(1*BRIGHTNESS/8)},
   {0, BRIGHTNESS, BRIGHTNESS},
   {0, int(1*BRIGHTNESS/4), BRIGHTNESS},
   {0, 0, BRIGHTNESS},
   {int(3*BRIGHTNESS/12), 0, BRIGHTNESS},
   {BRIGHTNESS, 0, BRIGHTNESS},
   {BRIGHTNESS, 0, int(1*BRIGHTNESS/8)}
  };
const int COLOR_N = sizeof(COLOR_LIST) / sizeof(COLOR_LIST[0]);

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
void set_color_and_show(int color_i, unsigned long delay_msec=0) {
  if (color_i >= COLOR_N) {
    color_i %= COLOR_N;
  }

  for (int i=0; i < PIXELS_N; i++) {
    pixels.setColor(i,
                    COLOR_LIST[color_i][0],
                    COLOR_LIST[color_i][1],
                    COLOR_LIST[color_i][2]);

    color_i = (color_i + 1) % COLOR_N;

    if ( delay_msec > 0 ) {
      pixels.show();
      delay(delay_msec);
    }
  } // for(i)

  if (delay_msec == 0) {
    pixels.show();
  }
}

/**
 *
 */
void setup() {
  //  pinMode(PIN_GND, OUTPUT);
  //  digitalWrite(PIN_GND, LOW);
  
  //  pinMode(PIN_ONBOARD_LED, OUTPUT);
  //  digitalWrite(PIN_ONBOARD_LED, HIGH);

  pixels.clear();
  pixels.show();


  //set_color_and_show(0, LOOP_DELAY);
  //set_colorHSV_and_show(0, HUE_STEP, LOOP_DELAY);
}

/**
 *
 */
void loop() {
  static uint32_t hue = 0;

  //  digitalWrite(PIN_ONBOARD_LED, LOW);

  //set_color_and_show(color_i);
  //color_i = (color_i + COLOR_N - 1) % COLOR_N;
  set_colorHSV_and_show(hue, HUE_STEP);
  hue = (hue + HUE_STEP) % 0x10000;

  delay(LOOP_DELAY / 2);
  //  digitalWrite(PIN_ONBOARD_LED, HIGH);
  delay(LOOP_DELAY / 2);
}
