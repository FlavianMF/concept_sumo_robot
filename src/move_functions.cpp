#include "move_functions.h"

#include "Arduino.h"
#include "motors.h"
#include <QuickPID.h>

#include "tasks/mpu_task.h"
// #include "my_mpu.h"

static const char *TAG = "move_functions";

float accel_time = 0;
float slow_time = 0;

float linear_kp = 1;
float linear_ki = 0;
float linear_kd = 0;

static int16_t acl_z = 0;
static float accel_z = 0;
static float pid_error = 0;
static float accel_setpoint = 0;

void stop_motors()  {
  ESP_LOGV(TAG, "stop_motors");
  drive_motors(0, 0);
}

void forward_move(float pwm, float time) {
  ESP_LOGV( TAG, "Forward with %f for %f milliseconds", pwm, time);

  bool calibrate_mpu = true;
  vTaskResume(mpu_task_handle);
  xQueueSend(mpu_calibrate_queue, &calibrate_mpu, portMAX_DELAY);
  xQueueReceive(mpu_queue, &acl_z, portMAX_DELAY);
  
  // float right_pwm;
  // float left_pwm;

  QuickPID pid(&accel_z, &pid_error, &accel_setpoint);
  pid.SetTunings(linear_kp, linear_ki, linear_kd);
  pid.SetOutputLimits(-1, 1);
  pid.SetMode(QuickPID::Control::automatic);
  // pid.SetControllerDirection(QuickPID::Action::reverse);

  uint64_t delta;
  float ramp_pwm = pwm;
  uint64_t timer = millis();
  while (millis() < timer + time) {

    ESP_LOGD(TAG, "pid: %f  acl:%i  accel:%f", pid_error, acl_z, accel_z);

    delta = millis() - timer;
    if (accel_time != 0 && delta < accel_time) {
       ramp_pwm = map(delta, 0, accel_time, 0, pwm);
    } else if (slow_time != 0 && delta > time - slow_time) {
      ramp_pwm = map(delta, time - slow_time, time, pwm, 0);
    }

    pid.Compute();
    
    if (pid_error > 0) {
      drive_motors(ramp_pwm, ramp_pwm * (1 - pid_error));
    } else if (pid_error < 0) {
      drive_motors(ramp_pwm * (1 + pid_error), ramp_pwm);
    } else {
      drive_motors(ramp_pwm, ramp_pwm);
    }

    xQueueReceive(mpu_queue, &acl_z, 1);
    accel_z = (float)acl_z;
  }

  vTaskSuspend(mpu_task_handle);
  stop_motors();
}


