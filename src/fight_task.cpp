#include "tasks/fight_task.h"

#include "esp32-hal-log.h"

static const char *TAG = "fight_task";

TaskHandle_t fight_task_handle;

fight_function_t output;
fight_function_t search;

void fight_task(void *pvParameters) {
  ESP_LOGV(TAG, "fight_task");

  output();

  search();

  vTaskDelete(NULL);
}
