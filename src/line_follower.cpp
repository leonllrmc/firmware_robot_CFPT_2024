#include "line_follower.h"
#include "communication.h"
#include <Arduino.h>

#include "utils.h"

#include <math.h>

#include "BLESerial.h"

uint8_t lastRight, lastLeft;

void lineFollowStartup()
{
   //SerialBT.begin("ESP32_LLR_robot"); //Bluetooth device name

   sendCommand("PW" + String(BASE_SPEED*10));

   sendCommand("AC2");

   delay(200);

   uint8_t sensorReadingLine = getLineSensorReadingBlocking();

   uint8_t lineLeft = (sensorReadingLine & 0xF0) >> 4;
   uint8_t lineRight = reverseBits((sensorReadingLine & 0x0F));

   lastLeft = lineLeft;
   lastRight = lineRight;
}

bool lastDirection;


// TODO: implement PID system
void lineFollowerTick()
{
   uint8_t sensorReadingLine = getLineSensorReadingBlocking();

   uint8_t lineLeft = (sensorReadingLine & 0x0F);
   uint8_t lineRight = reverseBits(((sensorReadingLine & 0xF0) >> 4));



   //SerialBT.println("LR: " + String(lineLeft) + ", " + String(lineRight));

   
   if(lineLeft > lineRight) // select only the most used line
   {


      if((lineLeft - lastLeft) > FILTER_THRESHOLD)
      {
         lineLeft = (lineLeft - lastLeft);
      }

      uint8_t baseSpeed = getBaseSpeed(lineLeft);
      float angleCorrection = getAngleCorrection(baseSpeed, lineLeft,  'L')/4;

      //SerialBT.println("Going left: " + String(angleCorrection));

      sendCommand("PG" + String((int)(max(0.5-angleCorrection, 0.0)*baseSpeed)*10));

      sendCommand("PD" + String((int)(abs(0.5+angleCorrection)*baseSpeed)*10));

      lastDirection = true;
   }
   else if (lineLeft < lineRight)
   {
      if((lineRight - lastRight) > FILTER_THRESHOLD)
      {
         lineRight = (lineRight + lastRight)/2;
      }

      uint8_t baseSpeed = getBaseSpeed(lineRight);
      float angleCorrection = getAngleCorrection(baseSpeed, lineRight, 'R')/4;

      //SerialBT.println("Going right: " + String(angleCorrection));

      sendCommand("PD" + String((int)(max(0.5-angleCorrection, 0.0)*baseSpeed)*10));

      sendCommand("PG" + String((int)(abs(0.5+angleCorrection)*baseSpeed)*10));

      lastDirection = false;
   }           
   else if(sensorReadingLine == 0x00)
   {
      // quited track
      if(lastDirection)
      {
         // recover to left
         sendCommand("PD" + String((int)(abs(0.25+(RECOVERY_COEFF/4))*RECOVERY_SPEED)*10));

         sendCommand("PG" + String((int)(max(0.25-(RECOVERY_COEFF/4), 0.0)*RECOVERY_SPEED)*10));
      }
      else
      {
         // recover to right
         sendCommand("PD" + String((int)(max(0.25-(RECOVERY_COEFF/4), 0.0)*RECOVERY_SPEED)*10));

         sendCommand("PG" + String((int)(abs(0.25+(RECOVERY_COEFF/4))*RECOVERY_SPEED)*10));
      }

      lastRight = lineRight;
      lastLeft = lineLeft;
   }
   else if(lineLeft == lineRight)
   {
      sendCommand("PD" + String(SPEED_COEFF_0*10));

      sendCommand("PG" + String(SPEED_COEFF_0*10));
   }
}


int getBaseSpeed(uint8_t lineUsed)
{
   int coeffBit0 = (lineUsed & 0x01) ? 1 : 0;
   int coeffBit1 = ((lineUsed & 0x02) >> 1) ? 1 : 0;
   int coeffBit2 = ((lineUsed & 0x04) >> 2) ? 1 : 0;
   int coeffBit3 = ((lineUsed & 0x08) >> 3) ? 1 : 0;

   Serial.println(String(coeffBit0) + String(coeffBit1) + String(coeffBit2) + String(coeffBit3));

   if(getBitNumber(lineUsed) == 0)
   {
      Serial.print("Returning because no sensor triggered");
      return 0;
   }

   int baseSpeed = (((coeffBit0 * SPEED_COEFF_0) + (coeffBit1 * SPEED_COEFF_1) + (coeffBit2 * SPEED_COEFF_2) + (coeffBit3 * SPEED_COEFF_3)) / (int)getBitNumber(lineUsed));

   Serial.println("Returned baseSpeed of: " + String(baseSpeed) + ", with " + String(getBitNumber(lineUsed)) + "Sensor triggered");


   return baseSpeed;
}

float getAngleCorrection(uint8_t baseSpeed, uint8_t lineUsed, char dir='L')
{

   int coeffBit0 = (lineUsed & 0x01) ? 1 : 0;
   int coeffBit1 = ((lineUsed & 0x02) >> 1) ? 1 : 0;
   int coeffBit2 = ((lineUsed & 0x04) >> 2) ? 1 : 0;
   int coeffBit3 = ((lineUsed & 0x08) >> 3) ? 1 : 0;

   // should be inversed, if multiple line are triggered, the situation is less worst than
   // if only furthest if triggered

   if(getBitNumber(lineUsed) == 0)
   {
      return 0;
   }

   float angleCorrection = (((float)coeffBit0 * ANGLE_COEFF_0) + ((float)coeffBit1 * ANGLE_COEFF_1) + ((float)coeffBit2 * ANGLE_COEFF_2) + ((float)coeffBit3 * ANGLE_COEFF_3)) / (float)getBitNumber(lineUsed);

   // Serial.printf("Returned angleCoeff: %i, for baseSpeed of: %i", angleCorrection, baseSpeed);
   Serial.println("Returned angleCoeff: " + String(angleCorrection) +", for baseSpeed of: " + String(baseSpeed));

   //sendBLEText("Returned angleCoeff: " + String(angleCorrection) +", for baseSpeed of: " + String(baseSpeed) + "With sensors being" + String(coeffBit0) + String(coeffBit1) + String(coeffBit2) + String(coeffBit3) + "and direction being" + String(dir) + "\n\r");


   return angleCorrection;
}