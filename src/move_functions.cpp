#include "move_functions.h"

#include "Arduino.h"
#include "motors.h"
#include <QuickPID.h>

#include "tasks/mpu_task.h"
#include "bluetooth.h"

static const char *TAG = "move_functions";

float accel_time = 0;
float slow_time = 0;

float linear_kp = 1;
float linear_ki = 0;
float linear_kd = 0;

void stop_motors()  {
  ESP_LOGV(TAG, "stop_motors");
  drive_motors(0, 0);
}

void forward_move(float pwm, float time) {
  ESP_LOGV( TAG, "Forward with %f for %f milliseconds", pwm, time);

  float right_pwm;
  float left_pwm;

  int16_t acl_z = 0;
  float accel_z = 0;
  float pid_error = 0;
  float accel_setpoint = 0;

  bool calibrate_mpu = true;
  vTaskResume(mpu_task_handle);
  xQueueSend(mpu_calibrate_queue, &calibrate_mpu, portMAX_DELAY);
  xQueueReceive(mpu_queue, &acl_z, portMAX_DELAY);
  xQueueReceive(mpu_queue, &acl_z, portMAX_DELAY);

  QuickPID pid(&accel_z, &pid_error, &accel_setpoint);
  pid.SetTunings(linear_kp, linear_ki, linear_kd);
  pid.SetOutputLimits(-1, 1);
  pid.SetMode(QuickPID::Control::automatic);
  // pid.SetControllerDirection(QuickPID::Action::reverse);

  uint64_t delta;
  float ramp_pwm = pwm;
  uint64_t timer = millis();
  while (millis() < timer + time) {
    if (xQueueReceive(mpu_queue, &acl_z, 1) != pdTRUE) {
      continue;
    }
    accel_z = (float)acl_z;
    
    delta = millis() - timer;
    if (accel_time != 0 && delta < accel_time) {
       ramp_pwm = map(delta, 0, accel_time, 0, pwm);
    } else if (slow_time != 0 && delta > time - slow_time) {
      ramp_pwm = map(delta, time - slow_time, time, pwm, 0);
    }

    pid.Compute();
    
    if (pid_error > 0) {
      right_pwm = ramp_pwm;
      left_pwm = ramp_pwm * (1 - pid_error);
      // drive_motors(ramp_pwm, ramp_pwm * (1 - pid_error));
    } else if (pid_error < 0) {
      left_pwm = ramp_pwm;
      right_pwm = ramp_pwm * (1 + pid_error);
      // drive_motors(ramp_pwm * (1 + pid_error), ramp_pwm);
    } else {
      left_pwm = ramp_pwm;
      right_pwm = ramp_pwm;
      // drive_motors(ramp_pwm, ramp_pwm);
    }

    drive_motors(left_pwm, right_pwm);


    ESP_LOGD(TAG, "ac: %f pid: %f  l:%f  r:%f", accel_z, pid_error, left_pwm, right_pwm);
  }

  vTaskSuspend(mpu_task_handle);
  stop_motors();
  bluetooth.printf("End Move forward\n");
}


