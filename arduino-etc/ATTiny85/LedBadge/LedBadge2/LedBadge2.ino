/**
 * Copyright (c) 2022 Yoichi Tanibayashi
 */
#include <EEPROM.h>
#include "Button.h"
#include "Ytani_NeoPixel.h"
#include "Mode_Rainbow.h"

const unsigned long LOOP_DELAY = 0;  // ms
const unsigned long DEBOUNCE   = 50;  // ms

const uint8_t PIN_BTN = 3;
const uint8_t PIN_LEDS  = 4;
const uint8_t LEDS_N   = 10;
const uint8_t BRIGHTNESS_MAX = 255;
uint8_t CurBr = BRIGHTNESS_MAX / 4;

Ytani_NeoPixel   *Leds;
Button Btn(PIN_BTN, "Button");

// EEPROM offset
const int eepMode = 0; // uint8_t (1 byte)
const int eepBr   = eepMode + 1; // uint8_t (1 byte)
const int eepRainbowCont = eepBr + 1; // unsigned long (4 bytes)
const int eepRainbowSat = eepRainbowCont + 4; // uint8_t (1 byte)
const int eepSingleColorCont = eepRainbowSat + 1; // unsigned long (4 bytes)
const int eepSingleColorSat = eepSingleColorCont + 4; // uint8_t (1 byte)
const int eepCrossCont = eepSingleColorSat + 1; // unsigned long (4 bytes)
const int eepCrossSat = eepCrossCont + 4; // uint8_t (1 byte)

// Modes
Mode_Rainbow     *mode_rainbow;

ModeBase* Mode[] =
  {
   mode_rainbow
  };
const int ModeN = sizeof(Mode) / sizeof(Mode[0]);
uint8_t CurMode = 0;

/**
 * ATTiny85 の割り込みルーチン(固定)
 *
 *  PCINT0_vect: ピン変化割り込み要求0(固定)
 */
ISR(PCINT0_vect) {
  noInterrupts();
  btn_intr_hdr();
  interrupts();
}

/**
 *
 */
void btn_intr_hdr() {
  static unsigned long prev_ms = 0;
  unsigned long        cur_ms = millis();

  if ( cur_ms - prev_ms > DEBOUNCE ) {
    prev_ms = cur_ms;
  
    (void)Btn.get();
  }
} // btn_intr_hdr()

/**
 *
 */
void btn_loop_hdr() {
  if ( Mode[CurMode]->btn_loop_hdr() ) {
    return;
  }

  int n = Btn.get_click_count();

  if ( n == 2 ) {
    // change brightness
    CurBr /= 4;
    if ( CurBr == 0 ) {
      CurBr = BRIGHTNESS_MAX;
    }
    EEPROM.put(eepBr, CurBr);
    return;
  }

  if ( n > 2 ) {
    // change mode
    CurMode = (CurMode + 1) % ModeN;
    EEPROM.put(eepMode, CurMode);
    return;
  }
} // btn_loop_hdr()

/**
 *
 */
void setup() {
  randomSeed(analogRead(0));

  Leds = new Ytani_NeoPixel(LEDS_N, PIN_LEDS);
  Leds->clear();
  Leds->setColor(0, 0x0000ff);
  Leds->show();
  delay(1000);

  EEPROM.get(eepMode, CurMode);
  if ( CurMode >= ModeN ) {
    CurMode = 0;
    EEPROM.put(eepMode, CurMode);
  }

  EEPROM.get(eepBr, CurBr);
  if ( CurBr == 0 || CurBr > BRIGHTNESS_MAX ) {
    CurBr = BRIGHTNESS_MAX / 4;
    EEPROM.put(eepBr, CurBr);
  }

  mode_rainbow = new Mode_Rainbow(Leds, &Btn, eepRainbowCont, eepRainbowSat);
  Leds->clear();
  Leds->show();
  delay(1000);

  // 割込設定
  GIMSK |= (1 << PCIE);  // PCINT割り込み有効
  PCMSK = (1 << PCINT3); // Pin3の割り込み許可

  Leds->setColor(1, 0x0000ff);
  Leds->show();
  delay(1000);
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

  Mode[CurMode]->loop();
  Leds->show();
  interrupts();

  delay(LOOP_DELAY);
} // loop()
