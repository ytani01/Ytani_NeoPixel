//
// Copyright (c) 2021 Yoichi Tanibayashi
//
#ifndef _YTANI_NEOPIXEL_H_
#define _YTANI_NEOPIXEL_H_

#include <Adafruit_NeoPixel.h>

typedef uint16_t hue_t; // 0..0xFFFF
typedef uint16_t hue_deg_t; // 0..360
typedef uint8_t sat_t; // 0..0xFF
typedef uint8_t br_t; // 0..0xFF
typedef uint32_t rgb_t; // 0x000000..0xFFFFFF
typedef uint8_t rgb_r_t; // 0x00..0xFF
typedef uint8_t rgb_g_t; // 0x00..0xFF
typedef uint8_t rgb_b_t; // 0x00..0xFF

/**
 *
 */
class Ytani_NeoPixel {
public:
  static const hue_t HUE_MAX = 0xFFFF;
  static const hue_deg_t HUE_DEG_MAX = 359;
  static const sat_t SAT_MAX = 0xFF;
  static const br_t BR_MAX = 0xFF;

  uint8_t pixels_n = 0;
  uint8_t pin;

  Ytani_NeoPixel(uint8_t n, uint8_t pin);

  void clear() {
    this->_pixels->clear();
  }

  void show() {
    this->_pixels->show();
    interrupts();
  }
  
  void setBrightness(br_t br) {
    this->_pixels->setBrightness(br);
  }

  void setColor(int led_i, rgb_t col) {
    this->_pixels->setPixelColor(led_i, col);
  }
  void setColor(int led_i, rgb_r_t r, rgb_g_t g, rgb_b_t b) {
    this->_pixels->setPixelColor(led_i, r, g, b);
  }
  
  rgb_t getColor(int led_i) {
    return this->_pixels->getPixelColor(led_i);
  }

  rgb_t color(rgb_r_t r, rgb_g_t g, rgb_b_t b) {
    return this->_pixels->Color(r, g, b);
  }

  /**
   * @param hue 0..0xFFFF
   * @param sat 0..0xFF
   * @param br 0..0xFF
   */
  rgb_t colorHSV(hue_t hue, sat_t sat, br_t br);
  void setColorHSV(int led_i, hue_t hue, sat_t sat, br_t br);

  /**
   * @param hue_deg 0..359
   * @param sat 0..0xFF
   * @param br 0..0xFF
   */
  void setColorHSVdeg(int led_i, hue_deg_t hue_deg, sat_t sat, br_t br);

protected:
  Adafruit_NeoPixel *_pixels;
};
#endif // _YTANI_NEOPIXEL_H_
