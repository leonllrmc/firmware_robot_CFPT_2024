#include "HAL_commands.h"
#include "utils.h"

#include <Arduino.h>

#include "distance_sensor.h"

#include "utils.h"

//#include "accelerometer.h"

uint8_t servoAngles[2] = {0, 0};

void moveForward(int distance)
{
   sendCommandBlocking("AV" + String(distance));
}

void moveBackward(int distance)
{
   sendCommandBlocking("RE" + String(distance));
}

void turnLeft(int angle)
{
   sendCommandBlocking("GA" + String(angle));
}

void turnRight(int angle)
{
   sendCommandBlocking("DR" + String(angle));
}

void setServoAngle(uint8_t servoID, uint8_t angle)
{
   servoAngles[servoID - 1] = angle;

   //Serial.println("Send angle of " + String(angle) + " to servo N" + String(servoID));

   int usAngle = (int)map(angle, 0, 180, 500, 2500);

   String cmd;

   if(usAngle < 1000)
   {
      cmd = ("0" + String(usAngle));
   }
   else
   {
      cmd = String(usAngle);
   }


   sendCommand("SE" + String(servoID) + cmd);

   //Serial.println("Sent: SE" + String(servoID) + cmd);
}

void setServoAngleStep(uint8_t servoID, uint8_t angle, uint8_t stepAmount, uint8_t stepDelay)
{
   uint8_t currentAngle = servoAngles[servoID - 1];

   uint8_t error = (angle - currentAngle);
   //Serial.println("Error: " + String(error));

   float correction = error / stepAmount;
   //Serial.println("correction: " + String(correction));

   float currentCorrection = 0;

   for(uint8_t i = 0; i < stepAmount; i++)
   {
      currentCorrection += correction;
      //Serial.println("currentCorrection: " + String(currentCorrection));

      setServoAngle(servoID, currentAngle + (int)currentCorrection);

      delay(stepDelay);
   }
}

void moveToDistanceFromWall(int distance)
{
   int initialDistance = (getMesurementSensor0() + getMesurementSensor1())/2;

   Serial.println("got initial Distance of: " + String(initialDistance));

   int correction = (initialDistance - distance);

   Serial.println("Calculated correction of: " + String(correction) + " for distance " + String(distance));

   /*if(abs(correction) < 2)
   {
      return; // sufficient for now
   }*/

   if(correction >= 0)
   {
      moveForward(correction);

      Serial.println("Forward of : " + String(correction));
   }
   else
   {
      moveBackward(-correction);

      Serial.println("Backward of : " + String(-correction));
   }

   Serial.println("");

}


void getStraightWithWall()
{
   int distance0 = getMesurementSensor0();

   while(distance0 < 25) distance0 = getMesurementSensor0();
   delay(500);
   int distance1 = getMesurementSensor1();
   while(distance1 < 25) distance1 = getMesurementSensor1();

   

   Serial.println("got Distance 0|1 of: " + String(distance0) + " " + String(distance1));

   int angle = getAngleFrom2Sensor(distance0, distance1);

   Serial.println("Found angle of " + String(angle));


   /*if(abs(angle) < 2)
   {
      return; // sufficient for now
   }*/

   if(angle >= 0)
   {
      turnRight(angle);
   }
   else
   {
      turnLeft(-angle);
   }
}

/*
void turnRightGyroscopeCorrected(int angle)
{
   int angleBefore = accelerometerGetYaw();

   sendCommandBlocking("DR" + String(angle));

   int angleAfter = accelerometerGetYaw();

   // note: invert for left
   int difference = (angleAfter - angleBefore);

   Serial.println("Got error (in GyroscopeCorrected turn): " + String(difference));

   int correction = angle - difference;
   Serial.println("Found correction (in GyroscopeCorrected turn): " + String(correction));

   if(angle >= 0)
   {
      sendCommandBlocking("DR" + String(correction));

      Serial.println("Sent DR" + String(correction));
   }
   else
   {
      sendCommandBlocking("GA" + String(-correction));

      Serial.println("Sent GA" + String(-correction));
   }


}

// TODO
void turnLeftGyroscopeCorrected(int angle)
{
   int angleBefore = accelerometerGetYaw();

   sendCommandBlocking("DR" + String(angle));

   int angleAfter = accelerometerGetYaw();

   // note: invert for left
   int difference = (angleAfter - angleBefore);

   Serial.println("Got error (in GyroscopeCorrected turn): " + String(difference));

   int correction = angle - difference;
   Serial.println("Found correction (in GyroscopeCorrected turn): " + String(correction));

   if(angle >= 0)
   {
      sendCommandBlocking("DR" + String(correction));

      Serial.println("Sent DR" + String(correction));
   }
   else
   {
      sendCommandBlocking("GA" + String(-correction));

      Serial.println("Sent GA" + String(-correction));
   }


}

*/