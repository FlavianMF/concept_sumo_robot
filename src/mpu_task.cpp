#include "tasks/mpu_task.h"

#include "my_mpu.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"

static const char *TAG = "mpu_task";

TaskHandle_t mpu_task_handle = NULL;

QueueHandle_t mpu_queue;
QueueHandle_t mpu_calibrate_queue;

mpu_infos_t mpu_infos_origin;

void mpu_task(void *pvParameters) {
  ESP_LOGV(TAG, "Init mpu task");

  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  if (!setup_mpu()) {
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  } else {
    rgb_state_t rgb_state = STATUS_OK;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  }
  xSemaphoreGive(setup_mutex);

  bool is_calibrate_mpu = false;

  // bool mpu_status = (bool)pvParameters;

  // if (!mpu_status)
  //   vTaskDelete(mpu_task_handle);

  mpu_queue = xQueueCreate(1, sizeof(mpu_infos_t));
  mpu_calibrate_queue = xQueueCreate(1, sizeof(bool));
  // uint64_t timer = 0;

  // vTaskSuspend(NULL);
  while (true) {
    // ESP_LOGD(TAG, "Time between calls: %ld us\n", (micros() - timer));
    // timer = micros();
    xQueueReceive(mpu_calibrate_queue, &is_calibrate_mpu, 1);

    if (is_calibrate_mpu) {
      calibrate_mpu();
      is_calibrate_mpu = false;
    }

    update_mpu();

    mpu_infos_origin.aclX = (aa.x / 131);
    mpu_infos_origin.aclY = (aa.y / 131);
    mpu_infos_origin.aclZ = (aa.z / 131);

    mpu_infos_origin.angX = euler[1] * 180 / M_PI;
    mpu_infos_origin.angY = euler[2] * 180 / M_PI;
    mpu_infos_origin.angZ = euler[0] * 180 / M_PI;

    xQueueSend(mpu_queue, &mpu_infos_origin, portMAX_DELAY);
  }
}
