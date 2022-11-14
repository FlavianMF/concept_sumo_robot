#include "setup_tasks.h"

#include <Arduino.h>

static const char *TAG = "setup_tasks";

SemaphoreHandle_t setup_mutex;

#include "tasks/mpu_task.h"
#include "tasks/bluetooth_task.h"
#include "tasks/rgb_task.h"
#include "tasks/motors_task.h"
#include "tasks/edge_task.h"
#include "tasks/vl_task.h"
#include "tasks/ir_task.h"

void setup_tasks(void) {
  setup_mutex = xSemaphoreCreateMutex();
  if (setup_mutex == NULL)
    ESP_LOGE(TAG, "Setup mutex failed");

  xTaskCreatePinnedToCore(rgb_task, "rgb", 2048, NULL, 10, &rgb_task_handle, 1);
  xTaskCreatePinnedToCore(edge_task, "edges", 2048, NULL, 10, &edge_task_handle, 1);
  xTaskCreatePinnedToCore(vl_task, "vl", 2048, NULL, 10, &vl_task_handle, 1);
  xTaskCreatePinnedToCore(mpu_task, "mpu", 4096, NULL, 10, &mpu_task_handle, 1);

  xTaskCreatePinnedToCore(motors_task, "motors", 3072, NULL, 5, &motors_task_handle, 0);
  xTaskCreatePinnedToCore(ir_task, "ir", 2048, NULL, 1, &ir_task_handle, 0);
  xTaskCreatePinnedToCore(bluetooth_task, "bluetooth", 4096, NULL, 1, &bluetooth_task_handle, 0);
}
