//
// Copyright (c) 2022 Yoichi Tanibayashi
//
#include "Ytani_NeoPixel.h"
#include "PressureSensor.h"
#include "Button.h"

const int NEXT_LED_N_FACTOR = 80; // 0-99

// const uint8_t PIN_LED_BUILTIN = 5;  // LOLIN D32 (ESP32)
// const uint8_t PIN_LED_BUILTIN = 1;  // DigiSpark ATTiny85
// const uint8_t PIN_LED_BUILTIN = 13;  // Arduino Uno
// const uint8_t PIN_LED_BUILTIN = 17;  // Arduino Pro Micro (RX)
// const uint8_t PIN_LED_BUILTIN = 30;  // Arduino Pro Micro (TX)
const uint8_t PIN_LED_BUILTIN = 27;  // M5Stamp Pico (NeoPixel)

const uint8_t PIN_LED_OUT1 = 25;

const uint8_t PIN_SENSOR = 33;
PressureSensor *sensor;

const uint8_t  PIN_PIXEL[]    = {PIN_LED_OUT1};
//const uint8_t  PIN_PIXEL[]    = {PIN_LED_OUT1, PIN_LED_BUILTIN};
const uint8_t  PIXELS_N       = sizeof(PIN_PIXEL) / sizeof(PIN_PIXEL[0]);
const uint16_t LED_N          = 255;
const int      BRIGHTNESS     = 20;  // 1-255

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

Ytani_NeoPixel *Pixel[PIXELS_N];

const uint8_t PIN_BTN_BUILTIN = 39;

const uint8_t PIN_BTN1 = 32;
Button *BtnBuiltin, *BtnOut;

/**
 *
 */
void set_color_and_show(int color_i, int led_n, unsigned long delay_msec=0) {
  if (color_i >= COLOR_N) {
    color_i %= COLOR_N;
  }

  for (int p=0; p < PIXELS_N; p++) {
    Pixel[p]->clear();
  } // for(PIXELS_N)

  for (int i=0; i < led_n; i++) {
    for (int p=0; p < PIXELS_N; p++) {
      Pixel[p]->setColor(i,
                         COLOR_LIST[color_i][0],
                         COLOR_LIST[color_i][1],
                         COLOR_LIST[color_i][2]);
    }

    color_i = (color_i + 1) % COLOR_N;

    if ( delay_msec > 0 ) {
      for (int p=0; p < PIXELS_N; p++) {
        Pixel[p]->show();
      }
      delay(delay_msec);
    }
  } // for(i)

  if (delay_msec >= 0) {
    for (int p=0; p < PIXELS_N; p++) {
      Pixel[p]->show();
    }
  }
}

/**
 *
 */
void btn_intr_hdr_builtin() {
  String name = BtnBuiltin->get_name();
  ESP_LOGI("", "name=%s", name.c_str());
} // btn_intr_hdr_builtin()

/**
 *
 */
void btn_intr_hdr_out() {
  String name = BtnOut->get_name();
  ESP_LOGI("", "name=%s", name.c_str());
} // btn_intr_hdr_out()

/**
 *
 */
void btn_loop_hdr() {
  String name = BtnOut->get_name();
  ESP_LOGI("", "name=%s", name.c_str());
} // btn_loop_hdr()

/**
 *
 */
void setup() {
  Serial.begin(115200);
  delay(500);

  sensor = new PressureSensor(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_9);

  ESP_LOGI("", "PIXELS_N=%d", PIXELS_N);
  for (int i=0; i < PIXELS_N; i++) {
    ESP_LOGI("", "i=%d", i);
    Pixel[i] = new Ytani_NeoPixel(LED_N, PIN_PIXEL[i]);
    Pixel[i]->clear();
    Pixel[i]->show();
  } // for(PIXELS_N)
  set_color_and_show(0, 5);

  //BtnBuiltin = new Button(PIN_BTN_BUILTIN, "BtnBuiltin");
  //attachInterrupt(PIN_BTN_BUILTIN, btn_intr_hdr_builtin, FALLING);

  BtnOut = new Button(PIN_BTN1, "BtnOut");
  attachInterrupt(PIN_BTN1, btn_intr_hdr_out, FALLING);
}

/**
 *
 */
void loop() {
  static int color_i = 1;
  static int next_led_n = 0;
  int val, led_n;

  val = sensor->get();
  if ( val < 2 ) {
    ESP_LOGD("", "val %d", val);
  } else {
    ESP_LOGI("", "val %d", val);
  }
  led_n = val / 25 - 2;
  if ( led_n < next_led_n ) {
    led_n = next_led_n;
  }

  set_color_and_show(color_i, led_n);

  color_i = (color_i + COLOR_N - 1) % COLOR_N;
  next_led_n = led_n * NEXT_LED_N_FACTOR / 100;
  
  delay(LOOP_DELAY);
}
