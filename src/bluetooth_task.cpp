#include "tasks/bluetooth_task.h"

#include "bluetooth.h"
#include "tasks/mpu_task.h"

TaskHandle_t bluetooth_task_handle;

QueueHandle_t bluetooth_queue;

static const char *TAG = "bluetooth_task";

void bluetooth_task(void *pvParameters) {
  uint16_t aclZ = 0;

  // vTaskSuspend(NULL);
  while (true) {
    if (bluetooth.connected()) {
      xQueueReceive(mpu_queue, &aclZ, portMAX_DELAY);

      bluetooth.print("aclZ: ");
      bluetooth.println(aclZ);

      // ESP_LOGD(TAG, "\nBluetooth plotted with: %i", aclZ);
    }
    vTaskDelay(1);
  }
}
