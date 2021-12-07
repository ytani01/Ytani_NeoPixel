/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include "Button.h"
#include "Ytani_NeoPixel.h"
#include "Mode_SingleColor.h"

const unsigned long LOOP_DELAY = 100;  // ms
const unsigned long DEBOUNCE   = 100;  // ms

const uint8_t PIN_LEDS  = 4;
const uint8_t LEDS_N   = 7;
const uint8_t PIN_BUTTON = 3;

Ytani_NeoPixel Leds(LEDS_N, PIN_LEDS);
Button         Btn(PIN_BUTTON, "Button");

Mode_SingleColor mode_single_color(&Leds, &Btn);
ModeBase* Mode[] =
  {
   &mode_single_color
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

  cli();

  if ( cur_ms - prev_ms > DEBOUNCE ) {
    prev_ms = cur_ms;
  
    if ( Btn.get() ) {
      btn_intr_hdr();
    }
  }

  sei();
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
  if ( mode_single_color.btn_loop_hdr() ) {
    return;
  }

  int n = Btn.get_click_count();

  if ( n > 2 ) {
    CurMode++;
    if ( CurMode >= ModeN ) {
      CurMode = 0;
    }
    sei();
    return;
  }
} // btn_loop_hdr()

/**
 *
 */
void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  
  GIMSK |= (1 << PCIE);  // PCINT割り込み有効
  PCMSK = (1 << PCINT3); // Pin3の割り込み許可

  Leds.clear();
  Leds.show();

  sei();  // 割り込み許可
} // setup()

/**
 *
 */
void loop() {
  if ( Btn.get() ) {
    cli();
    btn_loop_hdr();
    sei();
  }

  mode_single_color.loop();

  delay(LOOP_DELAY);
} // loop()
