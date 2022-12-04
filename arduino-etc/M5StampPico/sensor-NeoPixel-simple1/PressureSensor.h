/**
 * (c) 2021 Yoichi Tanibayashi
 *
 * !! Importan: for ADC1 only !!
 *
 *  ADC1 has 8 channels:
 *   ADC1_CHANNEL_0 GPIO36
 *   ADC1_CHANNEL_1 GPIO37
 *   ADC1_CHANNEL_2 GPIO38
 *   ADC1_CHANNEL_3 GPIO39
 *   ADC1_CHANNEL_4 GPIO32
 *   ADC1_CHANNEL_5 GPIO33
 *   ADC1_CHANNEL_6 GPIO34
 *   ADC1_CHANNEL_7 GPIO35
 * 
 *  ADC2 has 10 channels:
 *   ADC2_CHANNEL_0 GPIO4  A10
 *   ADC2_CHANNEL_1 GPIO0  A11
 *   ADC2_CHANNEL_2 GPIO2  A12
 *   ADC2_CHANNEL_3 GPIO15 A13
 *   ADC2_CHANNEL_4 GPIO13 A14
 *   ADC2_CHANNEL_5 GPIO12 A15   // Don't Use !!
 *   ADC2_CHANNEL_6 GPIO14 A16
 *   ADC2_CHANNEL_7 GPIO27 A17
 *   ADC2_CHANNEL_8 GPIO25 A18
 *   ADC2_CHANNEL_9 GPIO26 A19
 */
#ifndef PRESSURE_SENSOR_H
#define PRESSURE_SENSOR_H

//#include <Arduino.h>
#include <driver/adc.h>

class PressureSensor {
 public:
  static const adc_bits_width_t DEF_BIT_WIDTH = ADC_WIDTH_BIT_9;

  static unsigned long bitWidth2maxVal(adc_bits_width_t width);
  
  PressureSensor(adc1_channel_t adc_ch, adc_atten_t adc_atten,
                 adc_bits_width_t width=DEF_BIT_WIDTH);
  int get();
  
 private:
  adc1_channel_t  _adc_ch;
  adc_atten_t     _adc_atten;
  unsigned long   _max_val;
};
#endif // PRESSURE_SENSOR_H
