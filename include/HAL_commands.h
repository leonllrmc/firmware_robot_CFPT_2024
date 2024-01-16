#include "communication.h"

void moveForward(int distance);

void moveBackward(int distance);

void turnLeft(int angle);

void turnRight(int angle);

void setServoAngle(uint8_t servoID, uint8_t angle);

void setServoAngleStep(uint8_t servoID, uint8_t angle, uint8_t stepAmount, uint8_t stepDelay=50);