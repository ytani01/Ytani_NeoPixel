//
// Copyright (c) 2022 Yoichi Tanibayashi
//
#include "Ytani_NeoPixel.h"
#include "PressureSensor.h"

const uint8_t PIN_LED = 25;
const uint16_t LED_N_MAX = 16;
const int LED_N_FACTOR = 110;
Ytani_NeoPixel *Led;

const uint8_t PIN_SENSOR = 33;
PressureSensor *Sensor;

const unsigned long LOOP_DELAY = 100;  // ms


void setup() {
  Sensor = new PressureSensor(ADC1_CHANNEL_5, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_9);
  Led = new Ytani_NeoPixel(LED_N_MAX, PIN_LED);
  ESP_LOGI("", "setup");
}


void loop() {
  int val, led_n;

  val = Sensor->get();
  led_n = val / LED_N_FACTOR;
  ESP_LOGD("", "val=%d, led_n=%d", val, led_n);

  Led->clear();
  for (int i=0; i <= led_n; i++) {
    Led->setColor(i, 0x008080);
  } //for(i)
  Led->show();

  delay(LOOP_DELAY);
}
