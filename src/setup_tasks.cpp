#include "setup_tasks.h"

#include <Arduino.h>

#include "tasks/mpu_task.h"
#include "tasks/bluetooth_task.h"

void setup_tasks(void) {
  xTaskCreatePinnedToCore(mpu_task, "mpu", 4096, NULL, 10, &mpu_task_handle, 1);
  // xTaskCreatePinnedToCore(bluetooth_task, "bluetooth", 2048, NULL, 10, &bluetooth_task_handle, 1);
}
