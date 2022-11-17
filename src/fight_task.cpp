#include "tasks/fight_task.h"

#include <Arduino.h>
#include "bluetooth.h"
#include "esp32-hal-log.h"
#include "tasks/rgb_task.h"
#include "tasks/vl_task.h"
#include "tasks/edge_task.h"
#include "tasks/mpu_task.h"

static const char *TAG = "fight_task";

TaskHandle_t fight_task_handle;

fight_function_t output;
fight_function_t search;

void fight_task(void *pvParameters) {
  ESP_LOGD(TAG, "Initting fight...");
  bluetooth.printf("Initting fight...\n");

  uint64_t init_fight = millis();
  rgb_state_t rgb_state = PREPARING;
  xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  // vTaskResume(mpu_task_handle);
  // vTaskResume(vl_task_handle);
  // vTaskResume(edge_task_handle);
  bool calibrate_mpu = true;
  mpu_infos_t mpu_infos;
  xQueueSend(mpu_calibrate_queue, &calibrate_mpu, portMAX_DELAY);
  xQueueReceive(mpu_queue, &mpu_infos, portMAX_DELAY);
  xQueueReceive(mpu_queue, &mpu_infos, portMAX_DELAY);
  ESP_LOGD(TAG, "Time to delay: %d", (5000 - (millis() - init_fight)));
  vTaskDelay((pdMS_TO_TICKS(5000 - (millis() - init_fight))));
  bluetooth.printf("\n\nGO\n\n");

  output();
  
  // vTaskResume(edge_task_handle);
  // vTaskResume(vl_task_handle);

  search();

  vTaskDelete(NULL);
}
