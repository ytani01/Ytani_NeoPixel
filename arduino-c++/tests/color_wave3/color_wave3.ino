#include "Ytani_NeoPixel.h"

const uint8_t PIN_ONBOARD_LED = 5;  // LOLIN D32 (ESP32)
// const uint8_t PIN_ONBOARD_LED = 1;  // DigiSpark ATTiny85
// const uint8_t PIN_ONBOARD_LED = 13;  // Arduino Uno

const uint8_t PIN_PIXEL[]    = {4};
const uint8_t PIXELS_N       = sizeof(PIN_PIXEL) / sizeof(PIN_PIXEL[0]);
const uint8_t LED_N          = 255;
const int     BRIGHTNESS     = 32;  // 1-255

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

Ytani_NeoPixel pixels(LED_N, PIN_PIXEL[0]);
Ytani_NeoPixel *Pixel[PIXELS_N];

/**
 *
 */
void set_color_and_show(int color_i, unsigned long delay_msec=0) {
  if (color_i >= COLOR_N) {
    color_i %= COLOR_N;
  }

  for (int i=0; i < LED_N; i++) {
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
  Serial.begin(115200);
  delay(2000);
  Serial.printf("PIXELS_N=%d\n", PIXELS_N);

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, HIGH);

  Pixel[0] = new Ytani_NeoPixel(LED_N, PIN_PIXEL[0]);
  Serial.printf("AAA\n");
  Pixel[0]->clear();
  Pixel[0]->show();
  Serial.printf("BBB\n");
  delay(5000);
  
  Pixel[0]->setColor(0, 0xffffff);
  Pixel[0]->setColor(1, 0xffff00);
  Pixel[0]->show();
  Serial.printf("CCC\n");
  delay(5000);
  
  pixels.clear();
  pixels.show();
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
