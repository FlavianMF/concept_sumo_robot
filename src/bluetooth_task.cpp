#include "tasks/bluetooth_task.h"

#include "bluetooth.h"
#include "setup_tasks.h"

TaskHandle_t bluetooth_task_handle;

static const char *TAG = "bluetooth_task";

void bluetooth_task(void *pvParameters) {
  ESP_LOGV(TAG, "bluetooth task init");
  
  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  setup_bluetooth();
  xSemaphoreGive(setup_mutex);

  vTaskSuspend(NULL);
  while (true) {
    vTaskDelay(1);
  }
}
