#include <Arduino.h>

#include "MPU9250.h"

#include <Ticker.h>

#include "accelerometer.h"


#define MAGNETIC_DECLINATION 3.49

MPU9250 mpu;

Ticker updateTask(accelerometerUpdate, 10);

void initAccelerometer()
{
   Wire.begin(8, 9);

   delay(500);

      MPU9250Setting setting;

    // Sample rate must be at least 2x DLPF rate
    setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
    setting.gyro_fs_sel = GYRO_FS_SEL::G1000DPS;
    setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
    setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_250HZ;
    setting.gyro_fchoice = 0x03;
    setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_20HZ;
    setting.accel_fchoice = 0x01;
    setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;


        if (!mpu.setup(0x68, setting)) {  // change to your own address
         Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
    }


    mpu.selectFilter(QuatFilterSel::MADGWICK);
    mpu.setFilterIterations(15);


    // calibrated at 19/12/2023, new at 06/02/2024
    // magnetic calibration is not optimal, so it may need to be updated
    mpu.setAccBias(29.61, 16.31, 30.60);
    mpu.setGyroBias(-2.06, 1.80, -1.46);
    mpu.setMagBias(59.72, 118.02, -29.77);
    mpu.setMagScale(0.95, 0.86, 1.26);
    mpu.setMagneticDeclination(MAGNETIC_DECLINATION);

   updateTask.start();
}

void print_roll_pitch_yaw() {
   static int i;

   i++;

   if(i > 1)
   {
      i = 0;
   }

    Serial.println("Instruction N" + String(i));

    Serial.print("Yaw, Pitch, Roll: ");
    Serial.print(mpu.getYaw(), 2);
    Serial.print(", ");
    Serial.print(mpu.getPitch(), 2);
    Serial.print(", ");
    Serial.println(mpu.getRoll(), 2);

   Serial.println("");

   Serial.print("Quaternion X, Y, Z, W: ");
   Serial.print(mpu.getQuaternionX(), 2);
   Serial.print(", ");
   Serial.print(mpu.getQuaternionY(), 2);
   Serial.print(", ");
   Serial.print(mpu.getQuaternionZ(), 2);
   Serial.print(", ");
   Serial.println(mpu.getQuaternionW(), 2);

   Serial.println("");
   Serial.println("");
}

bool accelerometerUpdate()
{
   return mpu.update();
}

void accelerometerTick()
{
   if(accelerometerUpdate())
   {
      print_roll_pitch_yaw();
   }
}

int accelerometerGetYaw()
{
   while(!accelerometerUpdate());

   return mpu.getYaw();
}
