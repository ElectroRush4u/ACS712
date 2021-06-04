/*
 * FILE NAME: ACS712-AC-Test.ino
 * AUTHOR   : Rafi
 * VERSION  : 1.0
 * DATE     : 02-June-2021
 * PURPOSE  : Measure current easier, using this ACS712 current sensor library.
 * SOURCE   : https://www.allegromicro.com/en/products/sense/current-sensor-ics/zero-to-fifty-amp-integrated-conductor-sensor-ics/acs712
 */

#include <ACS712XX.h>

/* Description : The constructor of ACS712 current sensor. Create an object to use this ACS712 library.
 * Argument (s): Sensor type, Sensor pin, ADC reference voltage, ADC full-scale output.
 * Default  (s): ADC reference voltage -> 5, ADC full scale output -> 1023
 */
ACS712XX ACS712(ACS712_05B,A0);
//ACS712XX ACS712(ACS712_20A,A0);
//ACS712XX ACS712(ACS712_30A,A0);

void setup() {
  Serial.begin(9600);
  Serial.println(F("ACS712_05B Current Sensor Testing..."));
  Serial.println();
  
  Serial.print(F("Current Source Type:"));
  Serial.println(F(" AC"));
  Serial.println();
  
  Serial.print(F("Present Offset:"));
  Serial.println(ACS712.getOffset());       // Show you the present offset
  Serial.println();
  
  Serial.println(F("Auto Calibrating Current Sensor..."));
  Serial.print(F("Calibrated Offset:"));
  float _offset = ACS712.autoCalibrate();   // Must ensure there no current passing through the sensors
  Serial.println(_offset);
  Serial.println();
  
  Serial.print(F("Present Sensitivity:"));
  Serial.print(ACS712.getSensitivity());  // Show you the present sensitivity
  Serial.println(F(" mV/A"));
  Serial.println();
  
//  Only change this value when you are very sure that the sensor's sensitivity is changed.
//  ACS712.setSensitivity(200);

// This reset() function will reset sensor offset and sensitivity to default.
  ACS712.reset();
}

void loop() { 
  Serial.print(F("Measured Current:"));
  
//  By default, this getAC() function returns the current measurement value computing
//  only one period of time at 50Hz AC Signal.
  Serial.print(ACS712.getAC());

//  You can choose any frequency between 1 to 200 Hz. And 
//  the number of the measurement period -> 0.5 to 20(at 50Hz).

//  If you use this number of the measurement periods is equal to 0.5 then, it only computes half a period of
//  AC signal. that reduces measurement time. For a 50Hz signal, we need around 10ms.
//  Serial.print(ACS712.getAC(60,0.5));
  Serial.println(F(" A"));
  delay(100);
}
