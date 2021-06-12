/*
 * FILE NAME: ACS712XX.cpp
 * AUTHOR   : Rafi
 * VERSION  : 1.0
 * DATE     : 01-June-2021
 * PURPOSE  : Measure current easier, using this ACS712 current sensor library.
 * SOURCE   : https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712
 */

#include "ACS712XX.h"


ACS712XX :: ACS712XX(ACS712XXType _type, uint8_t _pin, uint8_t _adc_ref, int _adc_fs) {
  type = _type;
  adc_fs = _adc_fs;
  
  // Define sensor pin
  ACS712XX_pin = _pin;

  // Define sensor offset
  offset = _adc_fs/2.0;
  
  // Define sensor sensitivity according to the sensor type.
  inv_sensitivity = 1000.0 / float(sensor_sen[_type]);  // V/A -> A/V

  results_adjuster = float(_adc_ref) / float(_adc_fs);  // V/ADC
  results_adjuster *= inv_sensitivity;                  // V/ADC x A/V -> A/ADC
}

float ACS712XX :: autoCalibrate() {  // required time, around 1 ms
  int _adc = 0,_sample = 0;
  while (_sample < 10) {
    _adc += analogRead(ACS712XX_pin);
    _sample ++;
  }
  
  offset = float(_adc) * 0.1; // average of 10 samples
  return offset;
}

/* Description  : This function measures the AC current.
 * Argument (s) : Frequency, Number of measurement period 
 * Default  (s) : Frequency -> 50, Number of measurement period -> 1.0
 * Arg.(s) Range: Frequency -> Around 1 to 200 Hz, Number of measurement period -> 0.5 to 20(for 50Hz)
 * Return       : AC current in Ampere
 */
float ACS712XX :: getAC(float _freq, float _n_total_period) {  // It can measure minimum 0.5 maximum
  float _signal_period = 1000000.0 / _freq;                    // Hz -> us
  _signal_period *= _n_total_period;
  
//  Here, +8us confirms the last iteration. According to Arduino documentation,
//  the micros() function has a resolution of 4us.
  unsigned long _total_time = (unsigned long)_signal_period + 8;
  unsigned long _start_time = micros();

  unsigned long _adc_sqr_sum = 0;
  unsigned int _adc_count = 0;
  long _adc;

  while (micros() - _start_time < _total_time) { // it capable for maximum 4096 samples.
    _adc_count++;
    _adc = analogRead(ACS712XX_pin) - offset;
    _adc_sqr_sum += _adc*_adc;
  }
  
  float _avg_adc_sqr_sum = _adc_sqr_sum / (float)_adc_count;
  float _rms_current = sqrt(_avg_adc_sqr_sum) * results_adjuster;   // ADC x A/ADC -> A
  return _rms_current;
}

float ACS712XX :: getDC(int _count) {  // required time, around 1 ms of default 10 samples.
  int _adc = 0, _sample = 0;
  while (_sample < _count) {
    _adc += analogRead(ACS712XX_pin);
    _sample ++;
  }
  
  float _adc_avg = (float(_adc) / float(_count)) - offset;  // average of 10 samples and remove offset
  float _current = _adc_avg * results_adjuster;   // ADC x A/ADC -> A
  return _current;
}

void ACS712XX :: setSensitivity(float _sen) {
  results_adjuster /= inv_sensitivity;                  // A/ADC / A/V -> V/ADC
  inv_sensitivity = 1000.0 / _sen;                      // V/A -> A/V
  results_adjuster *= inv_sensitivity;                  // V/ADC x A/V -> A/ADC
}

void ACS712XX :: reset() {
  float _sen = float(sensor_sen[type]);
  setSensitivity(_sen);
  offset = adc_fs/2.0;
}
