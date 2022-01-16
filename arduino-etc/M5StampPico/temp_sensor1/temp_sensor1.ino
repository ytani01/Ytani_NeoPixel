/**
 * Copyright (c) 2022 Yoichi Tanibayashi
 */
#include <Adafruit_BME280.h>
#include "Ytani_NeoPixel.h"

const uint8_t I2CADDR_BME280 = 0x76;
const uint8_t PIN_LED = 27;
const uint8_t LED_BRIGHTNESS = 0x03;

Adafruit_BME280 bme;
Ytani_NeoPixel  *onBoardLed;

uint16_t temp2deg(float temp) {
  const uint16_t DEG_LOW = 240;
  const uint16_t DEG_HIGH = 0;
  const float TEMP_LOW = 18.0;
  const float TEMP_HIGH = 30.0;

  float a = (DEG_HIGH - DEG_LOW) / (TEMP_HIGH - TEMP_LOW);
  float b = DEG_LOW - a * TEMP_LOW;
  
  uint16_t deg = a * temp + b;
  // Serial.printf("a=%.2f b=%.2f deg=%d\n", a, b, deg);
  
  if ( temp > TEMP_HIGH ) {
    deg = DEG_HIGH;
  }
  if ( temp < TEMP_LOW ) {
    deg = DEG_LOW;
  }
  return deg;
} // temp2deg

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(1);
  }
  delay(1000);
  Serial.println("Start");
  
  onBoardLed = new Ytani_NeoPixel(1, PIN_LED);
  
  bme.begin(I2CADDR_BME280);
  bme.setSampling(Adafruit_BME280::MODE_FORCED);  // !! IMPORTANT !!
} // setup()

void loop() {
  /**
   * NeoPixelと共存するために毎回BME280の初期化が必要 !?
   */
  bme.begin(I2CADDR_BME280);
  bme.setSampling(Adafruit_BME280::MODE_FORCED);  // !! IMPORTANT !!

  bme.takeForcedMeasurement();  // !! IMPORTANT !!

  float temp = bme.readTemperature();
  float hum = bme.readHumidity();
  float pressure = bme.readPressure() / 100.0;

  uint16_t deg = temp2deg(temp);
  onBoardLed->setColorHSVdeg(0, deg, 0xff, LED_BRIGHTNESS);
  onBoardLed->show();
  
  Serial.printf("%.2f %.1f %0.2f %d\n", temp, hum, pressure, deg);

  delay(1000);
} // loop()
