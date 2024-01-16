#include <Arduino.h>
#include "communication.h"

#include "line_follower.h"

#include "enJumper.h"

#include "BLESerial.h"

#include "HAL_commands.h"

#define LED_PIN 8



unsigned char reverseBits(unsigned char num);

void setup() {
   // put your setup code here, to run once:
   Serial.begin(115200); // Serial for usb debug

   pinMode(LED_PIN, OUTPUT);

   initCommunication();
   emptySerialQueue();

   // BLECommInit();


   Serial.println("Begun USB serial port");

   // put back when hardware implementation of jumber if ready
   //while(getIfJumperPresent()) delay(100); // enable only when jumper removed

   sendCommand("AC2"); // set Acceleration to low

   sendCommand("AV10");

   lineFollowStartup();
}

void loop() {
   //lineFollowerTick();
   Serial.println("Tick");

   setServoAngle(1, 15);
   delay(1000);
   setServoAngleStep(1, 90, 5, 0);
   delay(3000);
  
   // BLECommTick();
   delay(10);
}

