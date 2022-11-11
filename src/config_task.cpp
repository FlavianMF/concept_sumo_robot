#include "tasks/config_task.h"

static const char *TAG = "config_task";

void config_task(void *pvParameters) {
  // ESP_LOGV(TAG, "config_task init");

  while(true){
    vTaskDelay(1);
  }
}
