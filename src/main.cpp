#include <Arduino.h>
#include "communication.h"

#include "line_follower.h"

#include "enJumper.h"

#include "BLESerial.h"

#include "HAL_commands.h"

#include "distance_sensor.h"

#include <Wire.h>

//#include "accelerometer.h"

#define LED_PIN 8



unsigned char reverseBits(unsigned char num);

void setup() {
   // put your setup code here, to run once:
   Serial.begin(115200); // Serial for usb debug

   //Wire.begin(8, 9);

   pinMode(LED_PIN, OUTPUT);

   initCommunication();
   emptySerialQueue();

   Serial.println("Inited communication");

   // BLECommInit();


   Serial.println("Begun USB serial port");

   // put back when hardware implementation of jumber if ready
   //while(getIfJumperPresent()) delay(100); // enable only when jumper removed

   sendCommand("AC2"); // set Acceleration to low


   //lineFollowStartup();
   //initAccelerometer();

   initDistanceSensors(0x29);

   Serial.println("Inited distance sensor");
}

void loop() {
   //lineFollowerTick();
   
   //moveForward(100);
   //accelerometerTick();

   //moveBackward(100);
   //accelerometerTick();
  
   // BLECommTick();

   /*moveToDistanceFromWall(100);

   delay(5000);

   moveToDistanceFromWall(120);

   delay(5000);

   moveToDistanceFromWall(70);

   delay(5000);*/

   getStraightWithWall();

   delay(500);

   delay(10);
}

