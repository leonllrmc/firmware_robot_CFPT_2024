#include "HAL_commands.h"
#include "utils.h"

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