/**
 * Copyright (c) 2022 Yoichi Tanibayashi
 */
#include <EEPROM.h>
#include "Button.h"
#include "Ytani_NeoPixel.h"
#include "ModeBase.h"
#include "ModeRainbow.h"
#include "ModeRing.h"

const unsigned long LOOP_DELAY = 0;  // ms
const unsigned long DEBOUNCE = 50;  // ms

const uint8_t PIN_BTN = 3;
const uint8_t PIN_LEDS = 4;
const uint8_t LEDS_N = 8;
const br_t BRIGHTNESS_MAX = 0xFF;
const br_t BRIGHTNESS_MIN = 0xF;
const br_t BRIGHTNESS_STEP = 4; // CurBr / BRIGHTNESS_STEP
br_t CurBr = BRIGHTNESS_MAX;

// devices
Ytani_NeoPixel *LEDs;
Button *Btn;

// EEPROM offset
const int eepMode = 0; // uint8_t (1 byte)
const int eepBr   = eepMode + 1; // uint8_t (1 byte)
const int eepModeBaseColorI = eepBr + 1; // unsigned long (4 bytes)
const int eepModeBaseCont = eepModeBaseColorI + 4; // (4 bytes)
const int eepModeRainbowColorI = eepModeBaseCont + 4; // (4 bytes)
const int eepModeRainbowCont = eepModeRainbowColorI + 4;
const int eepModeRingColorI = eepModeRainbowCont + 4; // (4 bytes)
const int eepModeRingCont = eepModeRingColorI + 4;

// Modes
ModeBase *modeBase;
ModeRainbow *modeRainbow;
ModeRing *modeRing;

ModeBase **Mode;
int ModeN;
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
  
    Btn->get();
  }
} // btn_intr_hdr()

/**
 *
 */
void btn_loop_hdr() {
  int n = Btn->get_click_count();

  if ( n == 2 ) {
    // change brightness
    CurBr /= BRIGHTNESS_STEP;
    if ( CurBr < BRIGHTNESS_MIN ) {
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

  Mode[CurMode]->btn_loop_hdr();
} // btn_loop_hdr()

/**
 *
 */
void setup() {
  LEDs = new Ytani_NeoPixel(LEDS_N, PIN_LEDS);
  Btn = new Button(PIN_BTN, "Button");

  modeBase = new ModeBase(LEDs, Btn,
                          eepModeBaseColorI, eepModeBaseCont);
  modeRainbow = new ModeRainbow(LEDs, Btn,
                                eepModeRainbowColorI, eepModeRainbowCont);
  modeRing = new ModeRing(LEDs, Btn,
                          eepModeRingColorI, eepModeRingCont);

  // 各モードのインスタンスを生成してから、配列を初期化する
  // 不確定長な配列の初期化はこうするしかない?
  static ModeBase *p[] = {modeBase, modeRainbow, modeRing};
  Mode = p;
  ModeN = sizeof(p) / sizeof(p[0]);

  EEPROM.get(eepMode, CurMode);
  if ( CurMode >= ModeN ) {
    CurMode = 0;
    EEPROM.put(eepMode, CurMode);
  }

  LEDs->setBrightness(BRIGHTNESS_MAX); // 明るさはHSVのVで制御する
  EEPROM.get(eepBr, CurBr);
  if ( CurBr == 0 || CurBr < BRIGHTNESS_MIN ) {
    CurBr = BRIGHTNESS_MAX / BRIGHTNESS_STEP;
    EEPROM.put(eepBr, CurBr);
  }

  // test LEDs
  LEDs->clear();
  for (int i=0; i < LEDS_N; i++) {
    LEDs->setColor(i, CurBr, CurBr, CurBr);
    LEDs->show();
    delay(200);
  }

  // 割込設定
  GIMSK |= (1 << PCIE);  // PCINT割り込み有効
  PCMSK = (1 << PCINT3); // Pin3の割り込み許可

  interrupts();
} // setup()

/**
 *
 */
void loop() {
  if ( Btn->get() ) {
    noInterrupts();
    btn_loop_hdr();
    interrupts();
  }

  Mode[CurMode]->loop();
  LEDs->show();

  delay(LOOP_DELAY);
} // loop()
