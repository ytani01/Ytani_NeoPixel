/*
 *
 */
#include "Button.h"
#include "Ytani_NeoPixel.h"

const uint8_t PIN_LED = 13;
const uint8_t PIN_BTN =  2;
const uint8_t PIN_LEDS = 3;

const unsigned long DELAY_MSEC = 5; // ms
const unsigned long DEBOUNCE = 10; // ms

Button Btn = Button(PIN_BTN, "BTN");
Ytani_NeoPixel Leds = Ytani_NeoPixel(7, PIN_LEDS);

boolean Intr = false;

/**
 *
 */
void btn_intr_hdr() {
  static unsigned long prev_msec = 0;
  unsigned long cur_msec = millis();

  if ( cur_msec - prev_msec < DEBOUNCE ) {
    return;
  }
  prev_msec = cur_msec;

  if ( Btn.get() ) {
    boolean val = Btn.get_value();
    Intr = true;
  }
}

/**
 *
 */
void setup() {
  Serial.begin(115200);
  while ( !Serial ) {
    delay(1);
  }
  Serial.println("start");

  Leds.clear();
  Leds.setColor(0, 0x0000ff);
  Leds.show();
  interrupts();

  attachInterrupt(digitalPinToInterrupt(PIN_BTN), btn_intr_hdr, CHANGE);
}

/**
 *
 */
void loop() {
  if ( Intr ) {
    Btn.print();
    Intr = false;
  }
  
  if ( Btn.get() ) {
    boolean val = Btn.get_value();
    Btn.print();
    Leds.clear();
    Leds.setColor(0, 0xff0000);
  } else {
    Leds.clear();
    Leds.setColor(0, 0x00ff00);
  }
  Leds.show();
  interrupts();

  delay(DELAY_MSEC);
}
