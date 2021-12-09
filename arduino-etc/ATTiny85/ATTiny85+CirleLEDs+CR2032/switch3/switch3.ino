/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Button.h"
#include "Ytani_NeoPixel.h"
#include "Mode_SingleColor.h"
//#include "Mode_SingleColor2.h"

const unsigned long LOOP_DELAY = 10;  // ms
const unsigned long DEBOUNCE   = 100;  // ms

const uint8_t PIN_BTN = 3;
const uint8_t PIN_LEDS  = 4;
const uint8_t LEDS_N   = 7;

Mode_SingleColor mode_single_color;
Mode_SingleColor mode_single_color2;
Ytani_NeoPixel Leds(LEDS_N, PIN_LEDS);
Button         Btn(PIN_BTN, "Button");

ModeBase* Mode[] =
  {
   &mode_single_color,
   &mode_single_color2
  };
const int ModeN = sizeof(Mode) / sizeof(Mode[0]);
uint8_t CurMode = 0;

/**
 * ATTiny85 の割り込みルーチン(固定)
 *
 *  PCINT0_vect: ピン変化割り込み要求0(固定)
 */
ISR(PCINT0_vect) {
  static unsigned long prev_ms = 0;
  unsigned long        cur_ms  = millis();

  noInterrupts();

  if ( cur_ms - prev_ms > DEBOUNCE ) {
    prev_ms = cur_ms;
  
    if ( Btn.get() ) {
      // btn_intr_hdr();
    }
  }

  interrupts();
}

/**
 *
 */
void btn_intr_hdr() {
  // do nothing
} // btn_intr_hdr()

/**
 *
 */
void btn_loop_hdr() {
  if ( mode_single_color.btn_loop_hdr(&Leds, &Btn) ) {
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
  pinMode(PIN_BTN, INPUT_PULLUP);
  
  GIMSK |= (1 << PCIE);  // PCINT割り込み有効
  PCMSK = (1 << PCINT3); // Pin3の割り込み許可

  Leds.clear();
  Leds.show();
  interrupts();
} // setup()

/**
 *
 */
void loop() {
  if ( Btn.get() ) {
    noInterrupts();
    btn_loop_hdr();
    interrupts();
  }

  mode_single_color.loop(&Leds, &Btn);
  interrupts();

  delay(LOOP_DELAY);
} // loop()
