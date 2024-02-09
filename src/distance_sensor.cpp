#include <VL53L0X.h>
#include <Wire.h>

// increasing it may increase readings accuracy but will take more time
#define SENSOR_TIMING_BUGET 200000

#define DISTANCE_OFFSET_FRONT 3
#define DISTANCE_OFFSET_BACK 3

VL53L0X distanceSensorBack;
VL53L0X distanceSensorFront;

#define TCAADDR 0x70

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

void initDistanceSensors(unsigned char adressBack=0x29, unsigned char adressFront=0x29) 
{
   Wire.begin(8, 9);

   tcaselect(0);

   delay(50);

   tcaselect(0);

   distanceSensorBack.setTimeout(500);
   //distanceSensorBack.address = adressBack;

   if (!distanceSensorBack.init())
   {
   // catch the error
    Serial.println("Error while init of Back distance sensor");
   }

  // settings for increasing range
   // lower the return signal rate limit (default is 0.25 MCPS)
  distanceSensorBack.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  distanceSensorBack.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  distanceSensorBack.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);

  //Serial.println("Inited sensor 0 with adress: " + String(distanceSensorBack.getAddress(), HEX));

   distanceSensorBack.setMeasurementTimingBudget(SENSOR_TIMING_BUGET);


   tcaselect(1);

   distanceSensorFront.setTimeout(500);
   //distanceSensorFront.address = adressFront;

   if (!distanceSensorFront.init())
   {
   // catch the error
    Serial.println("Error while init of Front distance sensor");
   }

   //Serial.println("Inited sensor 1 with adress: " + String(distanceSensorFront.getAddress(), HEX));

   distanceSensorFront.setMeasurementTimingBudget(SENSOR_TIMING_BUGET);

    // settings for increasing range
   // lower the return signal rate limit (default is 0.25 MCPS)
  distanceSensorFront.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  distanceSensorFront.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  distanceSensorFront.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
}


int getMesurementSensorBack() {
  Serial.println("Reading back sensor..."); 

  tcaselect(0);

  return (distanceSensorBack.readRangeSingleMillimeters() + DISTANCE_OFFSET_BACK);
  if (distanceSensorBack.timeoutOccurred()) { return -1; }
}

int getMesurementSensorFront() {
  Serial.println("Reading front sensor...");

  tcaselect(1);

  return (distanceSensorFront.readRangeSingleMillimeters() + DISTANCE_OFFSET_FRONT);
  if (distanceSensorFront.timeoutOccurred()) { return -1; }
}
