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
  Ytani_NeoPixel(uint8_t n, uint8_t pin) {
    this->_pixels = new Adafruit_NeoPixel(n, pin, NEO_GRB + NEO_KHZ800);
    this->_pixels->begin();
    this->_pixels->setBrightness(255);
    this->_pixels->clear();
    this->_pixels->show();
  }

  void clear() {
    this->_pixels->clear();
  }

  void show() {
    this->_pixels->show();
  }

  void setColor(int i, uint32_t col) {
    this->_pixels->setPixelColor(i, col);
  }

  void setColor(int i, uint8_t r, uint8_t g, uint8_t b) {
    this->_pixels->setPixelColor(i, r, g, b);
  }

  uint32_t getColor(int i) {
    return this->_pixels->getPixelColor(i);
  }

  uint32_t color(uint8_t r, uint8_t g, uint8_t b) {
    return this->_pixels->Color(r, g, b);
  }

private:
  Adafruit_NeoPixel *_pixels;
  uint8_t pixels_n = 0;
  uint8_t pin;
};
#endif // YTANI_NEOPIXEL_H
