#include "Ytani_NeoPixel.h"

const uint8_t PIN_ONBOARD_LED = 5;  // LOLIN D32 (ESP32)
// const uint8_t PIN_ONBOARD_LED = 1;  // DigiSpark ATTiny85
// const uint8_t PIN_ONBOARD_LED = 13;  // Arduino Uno

const uint8_t  PIN_PIXEL[]    = {4, 16};
const uint8_t  PIXELS_N       = sizeof(PIN_PIXEL) / sizeof(PIN_PIXEL[0]);
const uint16_t LED_N          = 255;
const int      BRIGHTNESS     = 32;  // 1-255

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

Ytani_NeoPixel pixels[] =
  {
   Ytani_NeoPixel(LED_N, PIN_PIXEL[0]),
   Ytani_NeoPixel(LED_N, PIN_PIXEL[1])
  };
Ytani_NeoPixel *Pixel[PIXELS_N];

/**
 *
 */
void set_color_and_show(int color_i, unsigned long delay_msec=0) {
  if (color_i >= COLOR_N) {
    color_i %= COLOR_N;
  }

  for (int i=0; i < LED_N; i++) {
    for (int p=0; p < PIXELS_N; p++) {
      pixels[p].setColor(i,
                         COLOR_LIST[color_i][0],
                         COLOR_LIST[color_i][1],
                         COLOR_LIST[color_i][2]);
    }

    color_i = (color_i + 1) % COLOR_N;

    if ( delay_msec > 0 ) {
      for (int p=0; p < PIXELS_N; p++) {
        pixels[p].show();
      }
      delay(delay_msec);
    }
  } // for(i)

  if (delay_msec == 0) {
    for (int p=0; p < PIXELS_N; p++) {
      pixels[p].show();
    }
  }
}

/**
 *
 */
void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.printf("PIXELS_N=%d\n", PIXELS_N);

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, HIGH);

  for (int p=0; p < PIXELS_N; p++) {
    pixels[p].clear();
    pixels[p].show();
  }
  // delay(1000);

  set_color_and_show(0, 5);
}

/**
 *
 */
void loop() {
  static int color_i = 1;

  digitalWrite(PIN_ONBOARD_LED, LOW);

  set_color_and_show(color_i);
  color_i = (color_i + COLOR_N - 1) % COLOR_N;

  delay(LOOP_DELAY / 2);
  digitalWrite(PIN_ONBOARD_LED, HIGH);
  delay(LOOP_DELAY / 2);
}
