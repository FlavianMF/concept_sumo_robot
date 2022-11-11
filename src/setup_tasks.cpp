#include "setup_tasks.h"

#include <Arduino.h>

static const char *TAG = "setup_tasks";

SemaphoreHandle_t setup_mutex;

#include "tasks/mpu_task.h"
#include "tasks/bluetooth_task.h"
#include "tasks/rgb_task.h"

void setup_tasks(void) {
  setup_mutex = xSemaphoreCreateMutex();
  if (setup_mutex == NULL)
    ESP_LOGE(TAG, "Setup mutex failed");

  xTaskCreatePinnedToCore(bluetooth_task, "bluetooth", 2048, NULL, 10, &bluetooth_task_handle, 0);

  xTaskCreatePinnedToCore(mpu_task, "mpu", 4096, NULL, 10, &mpu_task_handle, 1);
  xTaskCreatePinnedToCore(rgb_task, "rgb", 2048, NULL, 10, &rgb_task_handle, 1);
}
