#include "outputs.h"

#include "esp32-hal-log.h"
#include "bluetooth.h"
#include "motors.h"
#include "tasks/mpu_task.h"

static const char *TAG = "outputs";

void linear_output();
void right_front_output();

char *output_variables_commands[] = {
  (char*)"otime",
  (char*)"oangle",
  (char*)"lpwm",
  (char*)"apwm",
};

outputs_t outputs[] = {
  {
    .name = (char*)"Linear timed",
    .output = linear_output,
    .variables = {
      .time = 500,
      .angle = 0,
      .linear_pwm = 70,
      .angle_pwm = 50,
    }
  },
  {
    .name = (char*)"right->front",
    .output = right_front_output,
    .variables = {
      .time = 300,
      .angle = 45,
      .linear_pwm = 70,
      .angle_pwm = 50,
    }
  },
};
const int count_outputs = sizeof(outputs) / sizeof(outputs[0]);

output_function_t current_output;

float output_index = 0;

mpu_infos_t mpu_infos_output;

void set_output(uint8_t index) {
  ESP_LOGV(TAG, "set_output");

  if (index > count_outputs) {
    ESP_LOGE(TAG, "index out of range");
    return;
  }

  output_index = index;
  current_output = outputs[(int)output_index].output;
  ESP_LOGD(TAG, "Set output %d %s", output_index, outputs[(int)output_index].name);
}

void linear_output()  {
  ESP_LOGV(TAG, "linear_output");
  bluetooth.printf("\nOUT: linear_output");
  drive_motors(outputs[(int)output_index].variables.linear_pwm, outputs[(int)output_index].variables.linear_pwm);
  vTaskDelay(pdMS_TO_TICKS(outputs[(int)output_index].variables.time));
}

void right_front_output() {
  ESP_LOGV(TAG, "right_front_output");
  bluetooth.printf("\nOUT: right_front_output");

  float *angle = &outputs[(int)output_index].variables.angle;
  float *pwm_angle = &outputs[(int)output_index].variables.angle_pwm;
  float *linear_angle = &outputs[(int)output_index].variables.linear_pwm;
  float *time = &outputs[(int)output_index].variables.time;

  xQueueReceive(mpu_queue, &mpu_infos_output, portMAX_DELAY);
  int16_t target_z = mpu_infos_output.angZ + *angle;

  while(mpu_infos_output.angZ < target_z) {
    xQueueReceive(mpu_queue, &mpu_infos_output, portMAX_DELAY);
    drive_motors(*pwm_angle, -*pwm_angle);
  }

  drive_motors(*linear_angle, *linear_angle);
  vTaskDelay(pdMS_TO_TICKS(*time));
}