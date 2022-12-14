#ifndef __MY_MPU_H__
#define __MY_MPU_H__

#include <MPU6050_6Axis_MotionApps_V6_12.h>
#include <Wire.h>

#define MPU_INTERRUPT_PIN 23  // use pin 15 on ESP8266

MPU6050 mpu;

static const char *MPU_TAG = "mpu";

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
// uint8_t devStatus;  // return status after each device operation (0 =
// success,
//  !0 = error)

Quaternion q;    // [w, x, y, z]         quaternion container
VectorInt16 aa;  // [x, y, z]            accel sensor measurements
VectorInt16
    aaReal;  // [x, y, z]            gravity-free accel sensor measurements
VectorFloat gravity;     // [x, y, z]            gravity vector
uint8_t fifoBuffer[64];  // FIFO storage buffer
static float euler[3];   // [psi, theta, phi]    Euler angle container
float
    ypr[3];  // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

int16_t aclX = 0;
int16_t aclY = 0;
int16_t aclZ = 0;

int16_t angX = 0;
int16_t angY = 0;
int16_t angZ = 0;

uint8_t devStatus;

volatile bool mpuInterrupt;

void dmpDataReady() { mpuInterrupt = true; };

bool setup_mpu() {
  ESP_LOGV(MPU_TAG, "setting up mpu");

  Wire.begin();
  Wire.setClock(400000);  // 400kHz I2C clock. Comment this line if having
                          // compilation difficulties

  mpu.initialize();

  if (mpu.testConnection()) {
    ESP_LOGI(MPU_TAG, "connection established");
  } else {
    ESP_LOGE(MPU_TAG, "connection failed");
    return false;
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
    ESP_LOGI(MPU_TAG, "enabling interrupt detection (external %d interrupt)",
             digitalPinToInterrupt(MPU_INTERRUPT_PIN));
    // mpuInterrupt = false;
    attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady,
                    RISING);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use
    // it
    ESP_LOGI(MPU_TAG, "dmp ready, waiting for first interrupt...");
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    ESP_LOGE(MPU_TAG, "dmp initialization failed (code %d)", devStatus);
    return false;
  }

  ESP_LOGI(MPU_TAG, "mpu are setted");
  return true;
}

void update_mpu() {
  if (dmpReady) {
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetEuler(euler, &q);
      mpu.dmpGetGyro(&aa, fifoBuffer);
      // mpu.dmpGetAccel(&aa, fifoBuffer);
      // mpu.dmpGetGravity(&gravity, &q);
      // mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
      // mpu.dmpget

      aclX = (aa.x / 131);
      aclY = (aa.y / 131);
      aclZ = (aa.z / 131);

      angX = euler[1] * 180 / M_PI;
      angY = euler[2] * 180 / M_PI;
      angZ = euler[0] * 180 / M_PI;

      // mpu_infos_origin.aclX = (aa.x / 131);
      // mpu_infos_origin.aclY = (aa.y / 131);
      // mpu_infos_origin.aclZ = (aa.z / 131);

      // mpu_infos_origin.angX = euler[1] * 180 / M_PI;
      // mpu_infos_origin.angY = euler[2] * 180 / M_PI;
      // mpu_infos_origin.angZ = euler[0] * 180 / M_PI;

      // ESP_LOGD(MPU_TAG, "Acl X: %i\tAcl Y: %i\tAcl Z: %i", aclX, aclY, aclZ);

      mpu.dmpGetQuaternion(&q, fifoBuffer);
      // mpu.dmpGetGravity(&gravity, &q);
      // mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      // mpu.dmpGetQuaternion(&q, fifoBuffer);
      // mpu.dmpGetEuler(euler, &q);

      // angle_z = ypr[0] * 180 / M_PI;
      // angle_x = ypr[1] * 180 / M_PI;
      // angle_y = ypr[2] * 180 / M_PI;
      // angle_z = euler[0] * 180 / M_PI;
      // angle_x = euler[1] * 180 / M_PI;
      // angle_y = euler[2] * 180 / M_PI;

      // Serial.print("[ypr]\t");
      // Serial.print(ypr[0] * 180 / M_PI);
      // Serial.print("\t");
      // Serial.print(ypr[1] * 180 / M_PI);
      // Serial.print("\t");
      // Serial.print(ypr[2] * 180 / M_PI);
      // Serial.println();

      // Serial.println(ypr[0] * 180 / M_PI);
    }
  }
}

void calibrate_mpu(void) {
  ESP_LOGV(MPU_TAG, "Calibrating mpu");

  mpu.CalibrateAccel();
  mpu.CalibrateGyro();

  while (!dmpReady) {
  }

  ESP_LOGV(MPU_TAG, "mpu has calibrated");
}

#endif  // __MY_MPU_H__