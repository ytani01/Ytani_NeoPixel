/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#define _ATTINY85
// #define _ARDUINO_MICRO

#include "common.h"
#include "Button.h"
#include "Ytani_NeoPixel.h"
#include "Mode_SingleColor.h"
#include "Mode_Rainbow.h"

const unsigned long LOOP_DELAY = 10;  // ms
const unsigned long DEBOUNCE   = 100;  // ms

const uint8_t PIN_BTN = 3;
const uint8_t PIN_LEDS  = 4;
const uint8_t LEDS_N   = 7;

Ytani_NeoPixel   Leds(LEDS_N, PIN_LEDS);

Button           Btn(PIN_BTN, "Button");

Mode_SingleColor mode_single_color;
Mode_Rainbow     mode_rainbow;

ModeBase* Mode[] =
  {
   &mode_single_color,
   &mode_rainbow
  };
const int ModeN = sizeof(Mode) / sizeof(Mode[0]);
uint8_t CurMode = 0;

#ifdef _ATTINY85
/**
 * ATTiny85 の割り込みルーチン(固定)
 *
 *  PCINT0_vect: ピン変化割り込み要求0(固定)
 */
ISR(PCINT0_vect) {
  btn_intr_hdr();
}
#endif // _ATTINY85

/**
 *
 */
void btn_intr_hdr() {
  static unsigned long prev_ms = 0;
  unsigned long        cur_ms  = millis();

  noInterrupts();

  if ( cur_ms - prev_ms > DEBOUNCE ) {
    prev_ms = cur_ms;
  
#ifndef _ATTINY85
    Serial.println("btn_intr_hdr");
#endif // !_ATTINY85
  
    if ( Btn.get() ) {
    }
  }

  interrupts();
} // btn_intr_hdr()

/**
 *
 */
void btn_loop_hdr() {
#ifndef _ATTINY85
  Serial.println("btn_loop_hdr");
#endif // !_ATTINY85

  if ( Mode[CurMode]->btn_loop_hdr(&Leds, &Btn) ) {
  // if ( mode_single_color.btn_loop_hdr(&Leds, &Btn) ) {
    return;
  }

  int n = Btn.get_click_count();

  if ( n > 2 ) {
    CurMode++;
    if ( CurMode >= ModeN ) {
      CurMode = 0;
    }
    return;
  }
} // btn_loop_hdr()

/**
 *
 */
void setup() {
#ifndef _ATTINY85
  Serial.begin(115200);
  if ( !Serial ) {
    delay(1);
  }
  Serial.println("start");
#endif // !_ATTINY85
  
  Leds.clear();
  Leds.setColor(0, 0xff0000);
  Leds.show();

  pinMode(PIN_BTN, INPUT_PULLUP);

#ifdef _ATTINY85  
  GIMSK |= (1 << PCIE);  // PCINT割り込み有効
  PCMSK = (1 << PCINT3); // Pin3の割り込み許可
#endif // _ATTINY85

#ifdef _ARDUINO_MICRO
  attachInterrupt(digitalPinToInterrupt(PIN_BTN), btn_intr_hdr, CHANGE);
#endif // _ARDUINO_MICRO

  interrupts();
  delay(2000);
} // setup()

/**
 *
 */
void loop() {
#ifndef _ATTINY85
  Serial.println("loop");
#endif // !_ATTINY85

  if ( Btn.get() ) {
    noInterrupts();
    btn_loop_hdr();
    //interrupts();
  }

  Mode[CurMode]->loop(&Leds, &Btn);
  interrupts();

  delay(LOOP_DELAY);
} // loop()
