#include <Arduino.h>
#include <HardwareSerial.h>
#include "communication.h"

#include "accelerometer.h"

HardwareSerial RobotSerial(1);

void initCommunication()
{
   RobotSerial.begin(115200, SERIAL_8N1, 20, 21);

   delay(500);
}

void sendCommand(String cmd)
{
   RobotSerial.print(cmd + "\r");
}


void sendCommandBlocking(String cmd)
{
   emptySerialQueue();

   sendCommand(cmd);
   emptySerialQueue();
   delay(10);

   sendCommand("WR");

   waitForCmdReturnBlocking();
}

void emptySerialQueue()
{
   while(RobotSerial.available() > 0) RobotSerial.read();
}

void waitForCmdReturnBlocking()
{

   int timeoutCounter = 0;

   // wait until the char \n is recieved
   while(1)
   {
      accelerometerUpdate();

      if(RobotSerial.available())
      {
         if((char)RobotSerial.read() == '\n' || (char)RobotSerial.read() == '\r')
         {
            return;
         }
      }

      delay(2);
      timeoutCounter++;

      if(timeoutCounter > 500)
      {
         return; // timeout de 2s
      }
   }
}


uint8_t getLineSensorReadingBlocking()
{
   char cmd_buf[2] = {0, 0};
   uint8_t cmd_buf_pointer = 0;

   emptySerialQueue(); // pour éviter des mauvais résultats

   sendCommand("CL");

   delay(5);


   while(cmd_buf_pointer < 2)
   {
      while(RobotSerial.available() < 1);

      // Serial.println("Recieved char");

      cmd_buf[cmd_buf_pointer] = (char)RobotSerial.read();
      cmd_buf_pointer++;
   }

   return (uint8_t)strtol(cmd_buf, (char**)NULL, 16);
}