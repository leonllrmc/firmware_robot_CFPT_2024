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


uint8_t coreState = START;

unsigned char reverseBits(unsigned char num);


void setup() {
   pinMode(5, OUTPUT_OPEN_DRAIN);
   digitalWrite(5, 1);
   pinMode(4, OUTPUT_OPEN_DRAIN);
   digitalWrite(4, 1);

   // put your setup code here, to run once:
   Serial.begin(115200); ////Serial for usb debug

   //Wire.begin(8, 9);

   pinMode(LED_PIN, OUTPUT);

   pinMode(JUMPER_PIN, INPUT);



   initCommunication();
   emptySerialQueue();


  //Serial.println("Inited communication");

   // BLECommInit();


  //Serial.println("Begun USB//Serial port");

   // put back when hardware implementation of jumber if ready
   //while(getIfJumperPresent()) delay(100); // enable only when jumper removed

   sendCommand("AC2"); // set Acceleration to low, was 2


   //lineFollowStartup();
   //initAccelerometer();


  //Serial.println("Inited distance sensor");
  
  initDistanceSensors();

  setServoAngle(1, SERVO_ANGLE_LOW); // to tune


  delay(1500 + 2000 - 500);

  rainbowCycle();

  delay(400);

  setLedColorRGBW(0, 100, 200, 100);

   delay(400);

   while(getIfJumperPresent())
  { 
   delay(15);
   rainbowCycle();
  }

   moveForward(10);

   //moveForward(40);

   //setServoAngleStep(1, SERVO_ANGLE_HIGH, 3, 10); // for debug purpose when only 1 "base is present"

   //startLineFollowerBlocking();

   //coreState = DEPOSIT_1;
}



void coreStateMachineTick()
{
   switch(coreState)
   {
      case START:
         setSpeed(50);

         //moveToDistanceFromWallWithBack(92 - 9);

         coreState = INITIAL_PICKUP;
         break;

      case INITIAL_PICKUP:
         //turnLeft(69 + 2);// was 68 //turnLeftGyroscopeCorrected(90); // please change back..
         
         setSpeed(20);

         //was moveToDistanceFromWallWithFront 182, but laser detecteed the candelarbres
         moveToDistanceFromWallWithBack(604 + 10 + 5);// was 618 >, then 600 >, then 580 >, then 568 < //moveForward(344 - 182);

         setServoAngleStep(1, SERVO_ANGLE_HIGH, 3, 10); // to tune, steps were 2

         setSpeed(50);

         coreState = GOTO_LINEFOLLOWER;

         break;

      case GOTO_LINEFOLLOWER:
         //getMesurementSensorBack();
         //delay(50);

         moveToDistanceFromWallWithBack(410); // was 438//moveToDistanceFromWallWithFront(342);

         turnRight(70); // not precise turn neededed

         moveForward(350 - 100);//moveForward(267 - 92 + 140);
         //moveToDistanceFromWallWithBack(267);

         turnLeft(20 - 10);
         delay(20);

         moveForward(50);
         moveForward(50);


         //setSpeed(100);

         //turnRight(5);


         Serial.println("Started line follower...");

         /*sendCommand("ST");
         delay(10);*/
         sendCommand("DI0");
         delay(10);

         resetMotorBoard();
         delay(20);
         sendCommand("AC2");

         setServoAngleStep(1, SERVO_ANGLE_HIGH, 2, 10);

         startLineFollowerBlocking();

         coreState = DEPOSIT_1;

         break;

      case DEPOSIT_1:
         //sendCommand("DI0");
         //delay(40);
         //sendCommand("RST"); // (reseting the motor board should get rid of most problems..., if possible)
         //delay(500);
         
         delay(1800);
         //moveForward(10);
         //moveForward(10);

         resetMotorBoard();
         setSpeed(50);
         delay(20);
         
         setServoAngleStep(1, SERVO_ANGLE_HIGH, 2, 10);

         sendCommand("ST");
         delay(2);
         sendCommand("AC2");
         delay(2);
         sendCommand("DI0");
         delay(2);
         setSpeed(50);
         delay(50);


         Serial.println("ended line follower...");

         setLedColor(0x0F00FC00);
         delay(20); // debug

         sendCommandBlocking("AV10");
         delay(20);

         setLedColor(0x0FFC0000);
         delay(20); // debug


         //getMesurementSensorFront();
         //delay(50);

         moveToDistanceFromWallWithFront(330); // was 345
         delay(50);

         turnLeft(63 + 1 - 1 + 1 + 1 + 2); // was 70 < //turnLeftGyroscopeCorrected(90);

         setSpeed(40);

         //moveToDistanceFromWallWithBack(491 - 9);
         moveToDistanceFromWallWithFront(275 - (30 + 10 + 20));

         setServoAngleStep(1, SERVO_ANGLE_LOW - 2, 2, 10);
         delay(100);

         setSpeed(50);

         moveBackward(120 + 30 + 20); // to prevent robot hitting the cadelarbre

         //moveToDistanceFromWallWithBack(430);

         //moveToDistanceFromWallWithFront(330); // 
         //delay(20);

//         setSpeed(50);

         coreState = GOTO_PICKUP_LINE;

         break;

      case GOTO_PICKUP_LINE:

         //getMesurementSensorFront();
         //delay(50);

         //moveToDistanceFromWallWithFront(343);

         turnLeft(63); //turnRightGyroscopeCorrected(90);

         //getMesurementSensorBack();
         //delay(50);

         moveToDistanceFromWallWithBack(-100);
         delay(25);
         moveBackward(75);
         delay(50);
         moveBackward(10);
         delay(5);

         resetMotorBoard();
         sendCommand("AC2");
         setSpeed(50);

         moveForward(10);
         moveForward(40);

         moveToDistanceFromWallWithBack(92 - 9);

         setServoAngle(1, SERVO_ANGLE_LOW); // to tune

         coreState = PICKUP_2;

         break;

      case PICKUP_2:
         turnLeft(69 - (2 + 3 + 1 + 1));// was 68 //turnLeftGyroscopeCorrected(90); // please change back...

         setSpeed(30);

         //was moveToDistanceFromWallWithFront 182, but laser detecteed the candelarbres
         moveToDistanceFromWallWithBack(604 + 10 - (45 + 50 + 120) + 20 + 30);// was 618 >, then 600 >, then 580 >, then 568 < //moveForward(344 - 182);

         setServoAngleStep(1, SERVO_ANGLE_HIGH, 3, 10); // to tune, steps were 2

         setSpeed(50);

         coreState = GOTO_LINEFOLLOWER_2;
         break;

      case GOTO_LINEFOLLOWER_2:

         moveToDistanceFromWallWithBack(410); // was 438//moveToDistanceFromWallWithFront(342);

         turnRight(70 + 5 - 8 - 2); // not precise turn neededed

         moveForward(350);//moveForward(267 - 92 + 140);
         //moveToDistanceFromWallWithBack(267);

         turnLeft(8);

         moveForward(100);


         //§setSpeed(100);

         Serial.println("Started line follower...");

         /*sendCommand("ST");
         delay(10);*/
         sendCommand("DI0");
         delay(10);

         resetMotorBoard();
         delay(20);
         sendCommand("AC2");

         setServoAngleStep(1, SERVO_ANGLE_HIGH, 2, 10);

         startLineFollowerBlocking();

         coreState = DEPOSIT_2;

         break;

      case DEPOSIT_2:
         delay(1800);
         //moveForward(10);
         //moveForward(10);

         resetMotorBoard();
         delay(20);
         setSpeed(50);
         
         setServoAngleStep(1, SERVO_ANGLE_HIGH, 2, 10);

         sendCommand("ST");
         delay(2);
         sendCommand("AC2");
         delay(2);
         sendCommand("DI0");
         delay(2);
         setSpeed(50);
         delay(50);


         Serial.println("ended line follower...");

         setLedColor(0x0F00FC00);
         delay(20); // debug

         sendCommandBlocking("AV10");
         delay(20);

         setLedColor(0x0FFC0000);
         delay(20); // debug


         //getMesurementSensorFront();
         //delay(50);

         moveToDistanceFromWallWithFront(330); // was 345
         delay(50);

         turnLeft(63); // was 70 < //turnLeftGyroscopeCorrected(90);

         //moveToDistanceFromWallWithBack(491 - 9);
         moveToDistanceFromWallWithFront(275 - 30);

         setServoAngleStep(1, SERVO_ANGLE_LOW - 2, 2, 10);
         delay(100);

         moveBackward(50); // to prevent robot hitting the cadelarbre

         //moveToDistanceFromWallWithBack(430);
         //moveToDistanceFromWallWithFront(330);
         //delay(20); not really needed for this use case

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


