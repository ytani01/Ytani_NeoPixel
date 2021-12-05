/**
 *
 */
#include "Button.h"
#include "Ytani_NeoPixel.h"

const uint8_t PIN_BUTTON       = 3;
const uint8_t PIN_PIXEL        = 4;
const uint8_t PIXELS_N         = 8;
const int     BRIGHTNESS       = 64;  // 1-255
const int     HUE_STEP         = 0x1800;

const unsigned long DEBOUNCE   = 300;  // ms

const unsigned long LOOP_DELAY = 100;  // ms

Ytani_NeoPixel pixels(PIXELS_N, PIN_PIXEL);

Button Btn = Button(PIN_BUTTON, "Button");

static uint32_t Hue = 0;

/**
 *
 */
void set_colorHSV_and_show(uint32_t hue0, uint32_t hue_step,
                           unsigned long delay_msec=0) {
  for (int i=0; i < PIXELS_N; i++) {
    uint32_t col = pixels.colorHSV(hue0, 255, BRIGHTNESS);
    pixels.setColor(i, col);

    hue0 -= HUE_STEP;

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
 *
 */
void btn_loop_hdr(Button *btn) {
  int n = btn->get_click_count();
  if ( n > 0 ) {
    Hue = (Hue + HUE_STEP * n) % 0x10000;
  }
}

/**
 *
 */
void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  pixels.clear();
  pixels.show();
} // setup()

/**
 *
 */
void loop() {
  set_colorHSV_and_show(Hue, HUE_STEP);

  if (Btn.get()) {
    btn_loop_hdr(&Btn);
  }
  /*
  if (digitalRead(PIN_BUTTON) == LOW) {
    hue = (hue + HUE_STEP) % 0x10000;
  }
  */

  delay(LOOP_DELAY);
} // loop()
