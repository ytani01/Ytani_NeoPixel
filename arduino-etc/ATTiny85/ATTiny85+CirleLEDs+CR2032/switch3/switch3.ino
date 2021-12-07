/**
 * Copyright(c) 2021 Yoichi Tanibayashi
 */
#include "Button.h"
#include "Ytani_NeoPixel.h"

const unsigned long LOOP_DELAY = 100;  // ms
const unsigned long DEBOUNCE   = 100;  // ms

const uint8_t PIN_BUTTON       = 3;
const uint8_t PIN_PIXEL        = 4;
const uint8_t PIXELS_N         = 8;
const int     HUE_STEP         = 0x1800;

Ytani_NeoPixel pixels(PIXELS_N, PIN_PIXEL);
Button Btn = Button(PIN_BUTTON, "Button");

static uint32_t      Hue = 0x0000;
static uint8_t       Sat = 0xff;
static unsigned int  BRIGHTNESS_MAX = 255;
static unsigned int  CurBr = BRIGHTNESS_MAX >> 4;

const unsigned long HS[][2] =
  {
   {0x10000 *   0 / 360, 0x00}, // white
   {0x10000 *   0 / 360, 0xff}, // red
   {0x10000 *  10 / 360, 0xff}, // oringe
   {0x10000 *  60 / 360, 0xff}, // yellow
   {0x10000 * 120 / 360, 0xff}, // green
   {0x10000 * 180 / 360, 0xff}, // cyan
   {0x10000 * 240 / 360, 0xff}, // blue
   {0x10000 * 300 / 360, 0xff}  // purple
  };
int CurHS = 0;
int HS_N = sizeof(HS) / sizeof(HS[0]);

/**
 *
 */
void incHS() {
  CurHS++;
  if ( CurHS >= HS_N ) {
    CurHS = 0;
  }
}

/**
 * ATTiny85 の割り込みルーチン(固定)
 *
 *  PCINT0_vect: ピン変化割り込み要求0(固定)
 */
ISR(PCINT0_vect) {
  static unsigned long prev_ms = 0;
  unsigned long        cur_ms  = millis();

  if ( cur_ms - prev_ms < DEBOUNCE ) {
    return;
  }
  prev_ms = cur_ms;
  
  cli();
  if ( Btn.get() ) {
    btn_intr_hdr(&Btn);
  }
  sei();
}

/**
 *
 */
void btn_intr_hdr(Button *btn) {
  // do nothing
} // btn_intr_hdr()

/**
 *
 */
void btn_loop_hdr(Button *btn) {
  if ( btn->get_value() == Button::ON ) {
    if ( btn->is_repeated() ) {
      incHS();
    }
    return;
  }

  // Button::OFF
  int n = btn->get_click_count();

  if ( n == 1 ) {
    incHS();
    return;
  }
  
  if ( n == 2 ) {
    CurBr = CurBr >> 2;
    if ( CurBr <= 0 ) {
      CurBr = BRIGHTNESS_MAX;
    }
    return;
  }

  if ( n > 2 ) {
    CurBr = 0;
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

  pixels.clear();
  pixels.show();

  // sei();  // 割り込み許可
} // setup()

/**
 *
 */
void loop() {
  if ( Btn.get() ) {
    cli();
    btn_loop_hdr(&Btn);
    sei();
  }

  uint32_t col = pixels.colorHSV(HS[CurHS][0], HS[CurHS][1], CurBr);
  for (int i=0; i < PIXELS_N; i++) {
    pixels.setColor(i, col);
  } // for(i)
  pixels.show();

  delay(LOOP_DELAY);
} // loop()
