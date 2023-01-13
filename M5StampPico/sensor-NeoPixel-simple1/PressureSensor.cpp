/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "PressureSensor.h"

/**
 *
 */
unsigned long PressureSensor::bitWidth2maxVal(adc_bits_width_t width) {
  unsigned long max_val = 0;
  
  switch (width) {  // XXX
  case ADC_WIDTH_BIT_9:
    max_val = 511;
    break;
  default:
    max_val = 0;
    break;
  } // switch

  return max_val;
} // PressureSensor::bitWidth2maxVal()

/**
 * constructor
 */
PressureSensor::PressureSensor(adc1_channel_t adc_ch, adc_atten_t adc_atten,
                               adc_bits_width_t width) {
  this->_adc_ch = adc_ch;
  this->_adc_atten = adc_atten;
  this->_max_val = bitWidth2maxVal(width);
  
  adc1_config_width(width);
  adc1_config_channel_atten(this->_adc_ch, this->_adc_atten);
} // PressureSensor()

/**
 *
 */
int PressureSensor::get() {
  esp_err_t ret;
  int val;

  val = adc1_get_raw(this->_adc_ch);
  return (this->_max_val - val);
} // PressureSensor::get()
