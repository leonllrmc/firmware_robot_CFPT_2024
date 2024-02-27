#include "HAL_commands.h"
#include "utils.h"

#include <Arduino.h>

#include "distance_sensor.h"

#include "utils.h"

#include "accelerometer.h"

uint8_t servoAngles[2] = {0, 0};

void moveForward(int distance)
{
   sendCommandBlocking("AV" + String(distance));

   delay(50);
}

void moveBackward(int distance)
{
   sendCommandBlocking("RE" + String(distance));

   delay(50);
}

void turnLeft(int angle)
{
   String cmd = "GA";

   if(abs(angle) < 10)
   {
      cmd += String("0" + String(angle));
   }
   else
   {
      cmd += String(angle);
   }

   Serial.println("\n" + cmd);

   sendCommandBlocking(cmd);

   delay(50);
}

void turnRight(int angle)
{
   String cmd = "DR";

   if(abs(angle) < 10)
   {
      cmd += String("0" + String(angle));
   }
   else
   {
      cmd += String(angle);
   }

   Serial.println("\n" + cmd);

   sendCommandBlocking(cmd);

   delay(50);
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

void moveToDistanceFromWallWithBack(int distance)
{
   int initialDistance = getMesurementSensorBack();

   Serial.println("got initial Distance of: " + String(initialDistance));

   int correction = (initialDistance - distance);

   Serial.println("Calculated correction of: " + String(correction) + " for distance " + String(distance));

   /*if(abs(correction) < 2)
   {
      return; // sufficient for now
   }*/

   if(correction >= 0)
   {
      moveBackward(correction);

     Serial.println("Backward of : " + String(correction));
   }
   else
   {
      moveForward(-correction);

     Serial.println("Forward of : " + String(-correction));
   }

  Serial.println("");

}

void moveToDistanceFromWallWithFront(int distance)
{
   int initialDistance = getMesurementSensorFront();

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

void setSpeed(uint8_t speed)
{
   sendCommand("VI" + String(speed));
}


/*
void getStraightWithWall()
{
   int distance0 = getMesurementSensor0();

   delay(50); // try updating sensor value
   distance0 = getMesurementSensor0();

   while(distance0 < 15) 
   {
     //Serial.println("Got bad angle from sensor 0: " + String(distance0));

      distance0 = getMesurementSensor0();
   }

   delay(200);

   int distance1 = getMesurementSensor1();
   delay(50); // try updating sensor value
   distance1 = getMesurementSensor1();
   while(distance1 < 15) 
   {
     //Serial.println("Got bad angle from sensor 1: " + String(distance1));

      distance1 = getMesurementSensor1();
   }

   

  //Serial.println("got Distance 0|1 of: " + String(distance0) + " " + String(distance1));

   int angle = getAngleFrom2Sensor(distance0, distance1);

  //Serial.println("Found angle of " + String(angle));


   //if(abs(angle) < 2)
   //{
   //   return; // sufficient for now
   //}

   if(angle >= 0)
   {
      turnRight(angle);

     //Serial.println("Turning right of of " + String(angle));
   }
   else
   {
      turnLeft(-angle);

     //Serial.println("Turning left of of " + String(-angle));
   }
}
*/


void turnRightGyroscopeCorrected(int angle)
{
   Serial.println("starting corrected right turn of " + String(angle));

   int angleBefore = accelerometerGetYaw();

   Serial.println("Got " + String(angleBefore) + " as angle before");

   sendCommandBlocking("DR" + String((int)angle/2));

   int angleAfter = accelerometerGetYaw();

   Serial.println("Got " + String(angleAfter) + " as angle after");


   int difference = (float)(angleAfter - angleBefore)/2.5;

   if((angleAfter < 0) && (angleBefore < 0))
   {
      int difference = (float)(abs(angleBefore) - abs(angleAfter))/2.5; 
   }


   Serial.println("Got error (in GyroscopeCorrected turn): " + String(difference));

   int correction = ((angle/2) - abs(difference));

   /*if(abs(correction) > (angle * 0.5))
   {
      return; // NOTE: temporary fix...
   }*/
   Serial.println("Found correction (in GyroscopeCorrected turn): " + String(correction));

   if(correction >= 0)
   {
     sendCommandBlocking("DR" + String((int)(correction/2)));

     Serial.println("Sent DR" + String((int)(correction/2)));
   }
   else
   {
      sendCommandBlocking("GA" + String(-(int)(correction/2)));

     Serial.println("Sent GA" + String(-(int)(correction/2)));
   }

   //sendCommandBlocking("DR" + String((int)angle/2));

   Serial.println("");
}

// TODO
void turnLeftGyroscopeCorrected(int angle)
{
   Serial.println("starting corrected left turn of " + String(angle));


   int angleBefore = accelerometerGetYaw();

   Serial.println("Got " + String(angleBefore) + " as angle before");

   sendCommandBlocking("GA" + String((int)(angle/2)));

   Serial.println("Initial turn of " + String((int)(angle/2)));

   int angleAfter = accelerometerGetYaw();

   Serial.println("Got " + String(angleAfter) + " as angle after");


   int difference = (float)(angleAfter - angleBefore)/2.5;

   if((angleAfter < 0) && (angleBefore < 0))
   {
      int difference = (float)(abs(angleBefore) - abs(angleAfter))/2.5; 
   }


   Serial.println("Got error (in GyroscopeCorrected turn): " + String(difference));

   int correction = ((angle/2) - abs(difference));

   /*if(abs(correction) > (angle * 0.5))
   {
      return; // NOTE: temporary fix...
   }*/
  Serial.println("Found correction (in GyroscopeCorrected turn): " + String(correction));

   if(correction >= 0)
   {
     sendCommandBlocking("GA" + String((int)(correction/2)));

     Serial.println("Sent GA" + String((int)(correction/2)));
   }
   else
   {
     sendCommandBlocking("DR" + String(-(int)(correction/2)));

     Serial.println("Sent DR" + String(-(int)(correction/2)));
   }

   //sendCommandBlocking("GA" + String((int)angle/2));

   Serial.println("");
}
