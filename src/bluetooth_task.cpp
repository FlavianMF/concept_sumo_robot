#include "tasks/bluetooth_task.h"

#include "bluetooth.h"
#include "my_eeprom.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"

TaskHandle_t bluetooth_task_handle;

static const char *TAG = "bluetooth_task";

float bluetooth_pwm = 0;

static char *command;

void read_float_value(char *message) {
  ESP_LOGD(TAG, "value received: %s", message);

  if (!strcmp(message, "?")) {
    ESP_LOGV(TAG, "cancelling operation");

    bluetooth.printf("End operation\n");

    set_execute_command(execute_bluetooth_command);
    return;
  }

  float variable = (float)atof(message);

  update_prepared_variable(variable);
  set_execute_command(execute_bluetooth_command);
}

void execute_bluetooth_command(char *message) {
  ESP_LOGD(TAG, "command: %s", message);
  
  if (!strcmp(message, "F")) {
    ESP_LOGV(TAG, "Forward command received");
    // ESP_LOGD(TAG, "Pwm: %f", pwm);
    // command = PWM;
  } else {
    if (prepare_variable_update(message)) {
      set_execute_command(read_float_value);
    }
  }
}

void bluetooth_task(void *pvParameters) {
  ESP_LOGV(TAG, "bluetooth task init");
  
  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  if (!setup_bluetooth()){
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  } else {
    rgb_state_t rgb_state = STATUS_OK;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  }
  xSemaphoreGive(setup_mutex);

  set_execute_command(execute_bluetooth_command);

  vTaskSuspend(NULL);
  while (true) {  
    vTaskDelay(1);
  }
}
