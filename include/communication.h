#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

void initCommunication();
void sendCommand(String cmd);
void emptySerialQueue();

void sendCommandBlocking(String cmd); // always use for move commands if possible
bool waitForCmdReturnBlocking();

uint8_t getLineSensorReadingBlocking();

void rainbowCycle();
uint32_t Wheel(byte WheelPos);


void startLineFollowerBlocking();

void setLedColor(uint32_t color);

void resetMotorBoard();

void setLedColorRGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);