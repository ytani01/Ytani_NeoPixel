/**
 *

[LOLIN D32]
                           ----
                     3V --|    |-- GND
                  RESET --|    |-- PWM - 23
               36(IN)   --|    |-- PWM - 22
               39(IN)   --|    |-- PWM -  1
        A1C6 - 34(IN)   --|    |-- PWM -  3
   T9 - A1C4 - 32 - PWM --|    |-- PWM - 21
   T8 - A1C5 - 33 - PWM --|    |-- PWM - 19
 DAC1 - A2C8 - 25 - PWM --|    |-- PWM - 18
 DAC2 - A2C9 - 26 - PWM --|    |-- PWM -  5
   T7 - A2C7 - 27 - PWM --|    |-- PWM - 17
   T6 - A2C6 - 14 - PWM --|    |-- PWM - 16
   T5 - A2C5 - 12 - PWM --|    |-- PWM -  4 - A2C0 - T0
   T4 - A2C4 - 13 - PWM --|    |-- PWM -  0 - A2C1 - T1
                 ENABLE --|    |-- PWM -  2 - A2C2 - T2
                     5V --|    |-- PWM - 15 - A2C3 - T3
                     3V --|    |-- GND
                           ----

 */
#include "Ytani_NeoPixel.h"


const unsigned long LOOP_DELAY = 10;  // ms

const uint8_t PIN_PIXEL   = 4;
const uint8_t PIXELS_N    = 32;
const int     HUE_STEP    = 0x800;

Ytani_NeoPixel pixels(PIXELS_N, PIN_PIXEL);
uint16_t       hue = 0;
uint8_t        sat = 255;
uint8_t        val = 7;

/*
 *
 */
const uint8_t PIN_TOUCH1 = T7;
const uint8_t PIN_TOUCH2 = T3;
const uint8_t T_THR      = 30;
const int     DEBOUNCE   = 200;  // msec


boolean        touch1 = false;
unsigned long  prev_t1 = 0;
boolean        touch2 = false;
unsigned long  prev_t2 = 0;


/**
 *
 */
void show_all() {
  uint16_t h1 = hue;

  for (int i=0; i < PIXELS_N; i++) {
    pixels.setColor(i, pixels.colorHSV(h1, sat, val));

    h1 -= HUE_STEP;
  } // for(i)
  pixels.show();
}

/**
 *
 */
void intr_touch1() {
  unsigned long cur_t = millis();

  if (cur_t - prev_t1 < DEBOUNCE) {
    return;
  }

  if (touchRead(PIN_TOUCH1) > T_THR) {
    return;
  }

  Serial.printf("%d, %d\n", (int)touchRead(PIN_TOUCH1), (int)touchRead(PIN_TOUCH2));
  prev_t1 = cur_t;
  touch1 = true;
}

/**
 *
 */
void intr_touch2() {
  unsigned long cur_t = millis();

  if (cur_t - prev_t2 < DEBOUNCE) {
    return;
  }

  if (touchRead(PIN_TOUCH2) > T_THR) {
    return;
  }

  Serial.printf("%d, %d\n", touchRead(PIN_TOUCH1), touchRead(PIN_TOUCH2));
  prev_t2 = cur_t;
  touch2 = true;
}

/**
 *
 */
void setup() {
  Serial.begin(115200);

  touchAttachInterrupt(PIN_TOUCH1, intr_touch1, T_THR);
  touchAttachInterrupt(PIN_TOUCH2, intr_touch2, T_THR);
  pixels.clear();
  show_all();
}

/**
 *
 */
void loop() {
  // uint16_t t_val = touchRead(PIN_TOUCH1);

  if ( touch1 ) {
    hue += HUE_STEP;
  }
  if ( touch2 ) {
    if ( val == 0xFF ) {
      val = 7;
    }
    val = val <<= 1;
    val++;
  }

  if (touch1 || touch2) {
    Serial.printf("%d:%d, 0x%02X 0x%02X\n", touch1, touch2, hue, val);
    show_all();
  }

  touch1 = false;
  touch2 = false;

  delay(LOOP_DELAY);
}
