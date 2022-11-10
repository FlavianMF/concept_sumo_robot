#include "tasks/mpu_task.h"

#include "mpu.h"

static const char *TAG = "mpu_task";

TaskHandle_t mpu_task_handle = NULL;

QueueHandle_t mpu_queue;

void mpu_task(void *pvParameters) {
  ESP_LOGV(TAG,"Init mpu task");

  setup_mpu();

  bool mpu_status = (bool)pvParameters;

  if (!mpu_status)
    vTaskDelete(mpu_task_handle);

  mpu_queue = xQueueCreate(5, sizeof(float));

  while (true) {
    update_mpu();

    xQueueSend(mpu_queue, &aclZ, 1);
  }
}
