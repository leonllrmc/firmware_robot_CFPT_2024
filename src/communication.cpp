#include <Arduino.h>
#include <HardwareSerial.h>
#include "communication.h"

#include <Adafruit_NeoPixel.h>

#define PIN 7

#include "accelerometer.h"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_RGBW + NEO_KHZ800);

HardwareSerial RobotSerial(1);

int j;

void initCommunication()
{
  strip.begin();
  strip.setBrightness(30); // was 70, ajusted for test to prevent excessive battery usage
  strip.show(); // Initialize all pixels to 'off'

   RobotSerial.begin(115200, SERIAL_8N1, 20, 21);

   delay(500);
}


// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() {
  uint16_t i;

   j += 2;

    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();

    if(j >= 253) j = 0;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void sendCommand(String cmd)
{
   RobotSerial.print(cmd + "\r");
}


void sendCommandBlocking(String cmd)
{
   emptySerialQueue();

   rainbowCycle();

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