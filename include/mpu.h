#ifndef __MPU_H__
#define __MPU_H__
#include <MPU6050_6Axis_MotionApps_V6_12.h>
#include <Wire.h>

#define MPU_INTERRUPT_PIN 23  // use pin 15 on ESP8266

MPU6050 mpu;

#include "mpu.h"

const char *MPU_TAG = "mpu";

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
// uint8_t devStatus;  // return status after each device operation (0 =
// success,
//  !0 = error)

Quaternion q;            // [w, x, y, z]         quaternion container
VectorFloat gravity;     // [x, y, z]            gravity vector
uint8_t fifoBuffer[64];  // FIFO storage buffer
// static float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];  // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

uint8_t devStatus;

volatile bool mpuInterrupt;

void dmpDataReady() { mpuInterrupt = true; };

void setup_mpu() {
  ESP_LOGV(MPU_TAG, "setting up mpu");

  Wire.begin();
  Wire.setClock(400000);  // 400kHz I2C clock. Comment this line if having
                          // compilation difficulties

  mpu.initialize();

  if (mpu.testConnection()) {
    ESP_LOGI(MPU_TAG, "connection established");
  } else {
    ESP_LOGE(MPU_TAG, "connection failed");
    return;
  }

  pinMode(MPU_INTERRUPT_PIN, INPUT);

  ESP_LOGV(MPU_TAG, "Setting up dmp");
  devStatus = mpu.dmpInitialize();

  if (devStatus == 0) {
    ESP_LOGI(MPU_TAG, "DMP Initialization successful");

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(51);
    mpu.setYGyroOffset(8);
    mpu.setZGyroOffset(21);
    mpu.setXAccelOffset(1150);
    mpu.setYAccelOffset(-50);
    mpu.setZAccelOffset(1060);

    // Calibration Time: generate offsets and calibrate our MPU6050
    // mpu.CalibrateAccel(6);
    // mpu.CalibrateGyro(6);
    Serial.println();
    mpu.PrintActiveOffsets();

    // turn on the DMP, now that it's ready
    ESP_LOGV(MPU_TAG, "enabling dmp");
    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection
    ESP_LOGI(MPU_TAG, "enabling interrupt detection (external %d interrupt)", digitalPinToInterrupt(MPU_INTERRUPT_PIN));
    // Serial.print(
    //     F("[setup_mpu] Enabling interrupt detection (Arduino external "
    //       "interrupt "));
    // Serial.print(digitalPinToInterrupt(MPU_INTERRUPT_PIN));
    // Serial.println(F(")..."));
    mpuInterrupt = false;
    attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady,
                    RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use
    // it
    ESP_LOGI(MPU_TAG, "dmp ready, waiting for first interrupt...");
    // Serial.println(F("[setup_mpu] DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    ESP_LOGE(MPU_TAG, "dmp initialization failed (code %d)", devStatus);
    // Serial.print(F("[setup_mpu][E] DMP Initialization failed (code "));
    // Serial.print(devStatus);
    // Serial.println(F(")"));
  }

  Serial.println(F("[setup_mpu] end setup"));
}


#endif  // __MPU_H__