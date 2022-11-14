#include "outputs.h"

#include "esp32-hal-log.h"
#include "bluetooth.h"

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
}

void right_front_output() {
  ESP_LOGV(TAG, "right_front_output");
  bluetooth.printf("\nOUT: right_front_output");

}