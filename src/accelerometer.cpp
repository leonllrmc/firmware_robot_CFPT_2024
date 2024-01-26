#include <Arduino.h>

#include "MPU9250.h"

#define MAGNETIC_DECLINATION 2.49

MPU9250 mpu;

void initAccelerometer()
{
   Wire.begin(8, 9);

   delay(500);

   MPU9250Setting setting;
   setting.accel_fs_sel = ACCEL_FS_SEL::A16G;
   setting.gyro_fs_sel = GYRO_FS_SEL::G2000DPS;
   setting.mag_output_bits = MAG_OUTPUT_BITS::M16BITS;
   setting.fifo_sample_rate = FIFO_SAMPLE_RATE::SMPL_200HZ;
   setting.gyro_fchoice = 0x03;
   setting.gyro_dlpf_cfg = GYRO_DLPF_CFG::DLPF_41HZ;
   setting.accel_fchoice = 0x01;
   setting.accel_dlpf_cfg = ACCEL_DLPF_CFG::DLPF_45HZ;




   if (!mpu.setup(0x68, setting))  // change to your own address
   {
      Serial.println("Error: cannot connect accelerometer");
   }

    mpu.selectFilter(QuatFilterSel::MADGWICK);
    mpu.setFilterIterations(15);

    // calibrated at 19/12/2023
    // magnetic calibration is not optimal, so it may need to be updated
    mpu.setAccBias(12.81, 9.85, 48.60);
    mpu.setGyroBias(-1.99, 0.17, -2.55);
    mpu.setMagBias(49.69, -430.89, -365.53);
    mpu.setMagScale(1.04, 0.91, 1.07);
    mpu.setMagneticDeclination(MAGNETIC_DECLINATION);

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
      //print_roll_pitch_yaw();
   }
}

int accelerometerGetYaw()
{
   return mpu.getYaw();
}