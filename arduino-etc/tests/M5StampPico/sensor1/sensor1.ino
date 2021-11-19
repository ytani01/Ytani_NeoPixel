#include <Arduino.h>
#include "Ytani_NeoPixel.h"
#include "PressureSensor.h"

const uint8_t PIN_SENSOR = 33;
PressureSensor *sensor;

// const uint8_t PIN_ONBOARD_LED = 5;  // LOLIN D32 (ESP32)
// const uint8_t PIN_ONBOARD_LED = 1;  // DigiSpark ATTiny85
// const uint8_t PIN_ONBOARD_LED = 13;  // Arduino Uno
// const uint8_t PIN_ONBOARD_LED = 17;  // Arduino Pro Micro (RX)
// const uint8_t PIN_ONBOARD_LED = 30;  // Arduino Pro Micro (TX)
const uint8_t PIN_ONBOARD_LED = 27;  // M5Stamp Pico (NeoPixel)

const uint8_t  PIN_PIXEL[]    = {25};
//const uint8_t  PIN_PIXEL[]    = {25, PIN_ONBOARD_LED};
//const uint8_t  PIN_PIXEL[]    = {32, PIN_ONBOARD_LED};
//const uint8_t  PIN_PIXEL[]    = {32, 33};
const uint8_t  PIXELS_N       = sizeof(PIN_PIXEL) / sizeof(PIN_PIXEL[0]);
const uint16_t LED_N          = 150;
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

Ytani_NeoPixel Pixel[] =
  {
   Ytani_NeoPixel(LED_N, PIN_PIXEL[0]),
   Ytani_NeoPixel(LED_N, PIN_PIXEL[1]),
   Ytani_NeoPixel(LED_N, PIN_PIXEL[2])
  };
//Ytani_NeoPixel Pixel[PIXELS_N];

/**
 *
 */
void set_color_and_show(int color_i, unsigned long delay_msec=0) {
  if (color_i >= COLOR_N) {
    color_i %= COLOR_N;
  }

  for (int i=0; i < LED_N; i++) {
    for (int p=0; p < PIXELS_N; p++) {
      Pixel[p].setColor(i,
                         COLOR_LIST[color_i][0],
                         COLOR_LIST[color_i][1],
                         COLOR_LIST[color_i][2]);
    }

    color_i = (color_i + 1) % COLOR_N;

    if ( delay_msec > 0 ) {
      for (int p=0; p < PIXELS_N; p++) {
        Pixel[p].show();
      }
      delay(delay_msec);
    }
  } // for(i)

  if (delay_msec == 0) {
    for (int p=0; p < PIXELS_N; p++) {
      Pixel[p].show();
    }
  }
}

/**
 *
 */
void setup() {
  Serial.begin(115200);
  while ( !Serial ) {
    delay(10);
  }

  sensor = new PressureSensor(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_9);

  Serial.println("PIXELS_N=" + String(PIXELS_N));
  for (int i=0; i < PIXELS_N; i++) {
    Serial.println("i=" + String(i));
    //    Pixel[i] = Ytani_NeoPixel(LED_N, PIN_PIXEL[i]);
    Pixel[i].clear();
    Pixel[i].show();
  } // for(PIXELS_N)

  Serial.println("PIXELS_N=" + String(PIXELS_N));
  set_color_and_show(0, 5);
}

/**
 *
 */
void loop() {
  static int color_i = 1;
  int val;

  val = sensor->get();
  Serial.printf("val %d\n", val);

  set_color_and_show(color_i);
  color_i = (color_i + COLOR_N - 1) % COLOR_N;

  delay(LOOP_DELAY);
}
