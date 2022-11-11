#include "tasks/bluetooth_task.h"

#include "bluetooth.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"

TaskHandle_t bluetooth_task_handle;

static const char *TAG = "bluetooth_task";

// bool read_bluetooth_message(String *message) {
//   if (bluetooth.available()) {
//     char c = 0;
//     while (bluetooth.available()) {
//       c = bluetooth.read();
//       *message += c;
//       vTaskDelay(pdMS_TO_TICKS(3));
//     }
//     ESP_LOGD(TAG, "message: %s", message);
//     return true;
//   } else {
//     return false;
//   }
// }

void execute_bluetooth_command(char *message) {
  ESP_LOGD(TAG, "command: %s", message);
  
  if (!strcmp(message, "pwm")) {
    ESP_LOGD(TAG, "Pwm command received");
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
