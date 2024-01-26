#include <VL53L0X.h>
#include <Wire.h>

// increasing it may increase readings accuracy but will take more time
#define SENSOR_TIMING_BUGET 200000

VL53L0X distanceSensor0;
VL53L0X distanceSensor1;

void initDistanceSensors(unsigned char adress0=0x28, unsigned char adress1=0x29) 
{
   Wire.begin(8, 9);

   distanceSensor0.setTimeout(500);
   distanceSensor0.setAddress(adress0);

   if (!distanceSensor0.init())
   {
   // catch the error
   }

   distanceSensor0.setMeasurementTimingBudget(SENSOR_TIMING_BUGET);


   distanceSensor1.setTimeout(500);
   distanceSensor1.setAddress(adress1);

   if (!distanceSensor1.init())
   {
   // catch the error
   }

   distanceSensor1.setMeasurementTimingBudget(SENSOR_TIMING_BUGET);
}


int getMesurementSensor0() {
  return distanceSensor0.readRangeSingleMillimeters();
  if (distanceSensor0.timeoutOccurred()) { return -1; }
}

int getMesurementSensor1() {
  return distanceSensor1.readRangeSingleMillimeters();
  if (distanceSensor1.timeoutOccurred()) { return -1; }
}
