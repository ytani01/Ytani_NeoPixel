/**
 *
 */
#include "Button.h"
#include "Ytani_NeoPixel.h"

const unsigned long LOOP_DELAY = 100;  // ms

const uint8_t PIN_BUTTON       = 3;
const uint8_t PIN_PIXEL        = 4;
const uint8_t PIXELS_N         = 8;
const int     HUE_STEP         = 0x1800;

Ytani_NeoPixel pixels(PIXELS_N, PIN_PIXEL);
Button Btn = Button(PIN_BUTTON, "Button");

static uint32_t      Hue = 0;
static unsigned int  BRIGHTNESS_MAX = 255;
static unsigned int  Brightness = BRIGHTNESS_MAX >> 4;

/**
 *
 */
void set_colorHSV_and_show(uint32_t hue, uint32_t hue_step,
                           uint8_t br,
                           unsigned long delay_msec=0) {
  for (int i=0; i < PIXELS_N; i++) {
    uint32_t col = pixels.colorHSV(hue, 255, br);
    pixels.setColor(i, col);

    hue -= HUE_STEP;

    if ( delay_msec > 0 ) {
      pixels.show();
      delay(delay_msec);
    }
  } // for(i)

  if (delay_msec == 0) {
    pixels.show();
  }
} // set_colorHSV_and_show()

/**
 * ATTiny85 の割り込みルーチン(固定)
 *
 *  PCINT0_vect: ピン変化割り込み要求0(固定)
 */
ISR(PCINT0_vect) {
  cli();
  Btn.get();
  sei();
}

/**
 *
 */
void btn_loop_hdr(Button *btn) {
  int n = btn->get_click_count();

  if ( n == 1 || btn->is_repeated() ) {
    Hue = (Hue + HUE_STEP) % 0x10000;
    return;
  }

  if ( n == 2 ) {
    Brightness = Brightness >> 2;
    if ( Brightness <= 0 ) {
      Brightness = BRIGHTNESS_MAX;
    }
    return;
  }

  if ( n > 2 ) {
    Brightness = 0;
  }
}

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
  Btn.get();
  btn_loop_hdr(&Btn);
  set_colorHSV_and_show(Hue, HUE_STEP, Brightness, 0);

  delay(LOOP_DELAY);
} // loop()
