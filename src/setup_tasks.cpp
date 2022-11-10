#include "setup_tasks.h"

#include <Arduino.h>

#include "tasks/mpu_task.h"

void setup_tasks(void) {
  xTaskCreatePinnedToCore(mpu_task, "mpu", 1024, NULL, 10, &mpu_task_handle, 1);
}
