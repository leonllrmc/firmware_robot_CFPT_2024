
#define BASE_SPEED 35


typedef unsigned char uint8_t;

#define _Kp 1.3
#define _Ki 0.0
#define _Kd 0.0

#define PID_STRAIGHT_THRESHOLD 0.01

// tuned a bit for base speed of 20
//const unsigned char ANGLE_COEFF_0 = 0; -> 0%
//const unsigned char ANGLE_COEFF_1 = 1; -> 1/20 ->  5%
//const unsigned char ANGLE_COEFF_2 = 6; -> 30%
//const unsigned char ANGLE_COEFF_3 = 10;


const float ANGLE_COEFF_0 = 0.1;
const float ANGLE_COEFF_1 = 0.8;
const float ANGLE_COEFF_2 = 2.5;
const float ANGLE_COEFF_3 = 4;

const float RECOVERY_COEFF = 4.2;

#define FILTER_THRESHOLD 8

// NOTE: have each block be abs(most-less)-lesser
const int SPEED_COEFF_0 = 30;
const int SPEED_COEFF_1 = 25;
const int SPEED_COEFF_2 = 20;
const int SPEED_COEFF_3 = 15;

const unsigned char RECOVERY_SPEED = 20;

void lineFollowStartup();

void lineFollowerTick();

int getBaseSpeed(uint8_t lineUsed);

float getAngleCorrection(uint8_t baseSpeed, uint8_t lineUsed, char dir);

