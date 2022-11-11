#include "tasks/bluetooth_task.h"

#include "bluetooth.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"

TaskHandle_t bluetooth_task_handle;

static const char *TAG = "bluetooth_task";

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

  vTaskSuspend(NULL);
  while (true) {
    vTaskDelay(1);
  }
}
