//
// Copyright (c) 2021 Yoichi Tanibayashi
//
#ifndef YTANI_NEOPIXEL_H
#define YTANI_NEOPIXEL_H

#include <Adafruit_NeoPixel.h>

/**
 *
 */
class Ytani_NeoPixel {
public:
  uint8_t pixels_n = 0;
  uint8_t pin;

  Ytani_NeoPixel(uint8_t n, uint8_t pin) {
    this->pixels_n = n;
    this->pin = pin;
    this->_pixels = Adafruit_NeoPixel(n, pin, NEO_GRB + NEO_KHZ800);
    this->_pixels.begin();
    this->_pixels.setBrightness(255);
  }

  void clear() {
    this->_pixels.clear();
  }

  void show() {
    this->_pixels.show();
  }

  void setColor(int i, uint32_t col) {
    this->_pixels.setPixelColor(i, col);
  }

  void setColor(int i, uint8_t r, uint8_t g, uint8_t b) {
    this->_pixels.setPixelColor(i, r, g, b);
  }

  uint32_t getColor(int i) {
    return this->_pixels.getPixelColor(i);
  }

  uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
    return this->_pixels.Color(r, g, b);
  }

  /**
   * @param hue 0-65535(0xffff)
   * @param sat 0-255
   * @param val 0-255
   */
  uint32_t colorHSV(uint16_t hue, uint8_t sat, uint8_t val) {
    return this->_pixels.ColorHSV(hue, sat, val);
  }

private:
  Adafruit_NeoPixel _pixels;
};
#endif // YTANI_NEOPIXEL_H
