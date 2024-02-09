#include <Arduino.h>
#include "communication.h"

#include "line_follower.h"

#include "enJumper.h"

//#include "BLESerial.h"

#include "HAL_commands.h"

#include "distance_sensor.h"

#include <Wire.h>

#include "states.h"

#include "accelerometer.h"

#define LED_PIN 8

#define SERVO_ANGLE_LOW 4 // to tune
#define SERVO_ANGLE_HIGH 130 // to tune

unsigned char reverseBits(unsigned char num);


void setup() {
   // put your setup code here, to run once:
   Serial.begin(115200); ////Serial for usb debug

   //Wire.begin(8, 9);

   pinMode(LED_PIN, OUTPUT);

   pinMode(JUMPER_PIN, OUTPUT);

   initCommunication();
   emptySerialQueue();


  //Serial.println("Inited communication");

   // BLECommInit();


  //Serial.println("Begun USB//Serial port");

   // put back when hardware implementation of jumber if ready
   //while(getIfJumperPresent()) delay(100); // enable only when jumper removed

   sendCommand("AC1"); // set Acceleration to low, was 2


   //lineFollowStartup();
   //initAccelerometer();


  //Serial.println("Inited distance sensor");
  
  initDistanceSensors();

  setServoAngle(1, SERVO_ANGLE_LOW); // to tune


  delay(1500);
  while(getIfJumperPresent())
  { 
   delay(15);
   rainbowCycle();
  }

  moveForward(10);

  moveForward(40);
}


uint8_t coreState = START;

void coreStateMachineTick()
{
   switch(coreState)
   {
      case START:
         setSpeed(70);

         moveToDistanceFromWallWithBack(92 - 9);

         coreState = INITIAL_PICKUP;
         break;

      case INITIAL_PICKUP:
         turnLeft(69);// was 68 //turnLeftGyroscopeCorrected(90); // please change back...

         //was moveToDistanceFromWallWithFront 182, but laser detecteed the candelarbres
         moveToDistanceFromWallWithBack(604);// was 618 >, then 600 >, then 580 >, then 568 < //moveForward(344 - 182);

         setServoAngleStep(1, SERVO_ANGLE_HIGH, 3, 10); // to tune, steps were 2

         coreState = GOTO_LINEFOLLOWER;

         break;

      case GOTO_LINEFOLLOWER:
         //getMesurementSensorBack();
         //delay(50);

         moveToDistanceFromWallWithBack(410); // was 438//moveToDistanceFromWallWithFront(342);

         turnRight(58); // not precise turn neededed

         moveForward(350);//moveForward(267 - 92 + 140);
         //moveToDistanceFromWallWithBack(267);


         //setSpeed(100);

         //turnRight(5);

         Serial.println("Started line follower...");

         sendCommand("ST");
         delay(10);
         sendCommand("DI0");
         delay(10);

         sendCommandBlocking("LF");

         coreState = DEPOSIT_1;

         break;

      case DEPOSIT_1:
         sendCommand("ST");
         delay(10);
         sendCommand("DI0");
         delay(10);

         Serial.println("ended line follower...");

         sendCommand("AV10");
         delay(100);

         setSpeed(70);

         //getMesurementSensorFront();
         //delay(50);

         moveToDistanceFromWallWithFront(351); // was 345
         delay(50);

         turnLeft(74); // was 70 < //turnLeftGyroscopeCorrected(90);

         //getMesurementSensorBack();
         //delay(50);

         moveToDistanceFromWallWithBack(550);

         setSpeed(70);

         setServoAngleStep(1, SERVO_ANGLE_LOW, 2, 10);

         moveToDistanceFromWallWithBack(433);

         setSpeed(70);

         coreState = GOTO_PICKUP_LINE;

         break;

      case GOTO_PICKUP_LINE:

         //getMesurementSensorFront();
         //delay(50);

         moveToDistanceFromWallWithFront(343);

         turnRight(70); //turnRightGyroscopeCorrected(90);

         //getMesurementSensorBack();
         //delay(50);

         moveToDistanceFromWallWithBack(91);

         coreState = PICKUP_2;

         break;

      case PICKUP_2:
         turnLeft(70); //turnLeftGyroscopeCorrected(90);

         //getMesurementSensorFront();
         //delay(50);


         moveToDistanceFromWallWithFront(182);

         setServoAngleStep(1, SERVO_ANGLE_HIGH, 2, 10); // to tune

         coreState = GOTO_LINEFOLLOWER_2;
         break;

      case GOTO_LINEFOLLOWER_2:

         moveToDistanceFromWallWithFront(342);

         turnRight(70);

         //getMesurementSensorBack();
         //delay(50);

         moveToDistanceFromWallWithBack(267);

         setSpeed(100);

         sendCommandBlocking("LF");
         delay(600);
         sendCommandBlocking("");

         coreState = DEPOSIT_2;

         break;

      case DEPOSIT_2:
         sendCommand("DI0");

         sendCommand("AV10"); // just to wake up motor board...
         delay(50);

         setSpeed(70);

         moveToDistanceFromWallWithFront(340);

         turnLeft(70); //turnLeftGyroscopeCorrected(90);

         moveToDistanceFromWallWithFront(280);

         setSpeed(70);

         setServoAngleStep(1, SERVO_ANGLE_LOW, 2, 10);

         moveToDistanceFromWallWithFront(310);

         setSpeed(70);

         coreState = INTERRUPT_TIMER;

         break;


      case INTERRUPT_TIMER:

      setSpeed(100);

      moveBackward(510);

      break;

   }
}


void loop() {
   coreStateMachineTick();
}


