#include "tasks/edge_task.h"

#include <Arduino.h>

#include "edge_sensors.h"
#include "esp32-hal-log.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"

static const char *TAG = "edge_task";

TaskHandle_t edge_task_handle;
QueueHandle_t edge_queue;

float raw_edge_sensors = 1;

float right_edge_limit = 2048;
float left_edge_limit = 2048;

void edge_task(void *pvParameters) {
  ESP_LOGV(TAG, "edge_task");


  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  edge_queue = xQueueCreate(1, sizeof(edge_infos_t));
  if (edge_queue == NULL) {
    ESP_LOGE(TAG, "Queue not created");
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  }
  setup_edge_sensors();
  xSemaphoreGive(setup_mutex);

  edge_infos_t edge_infos;

  int right_edge;
  int left_edge;

  // vTaskSuspend(NULL);
  while (true) {
    right_edge = analogRead(right_edge_pin);
    left_edge = analogRead(left_edge_pin);

    // edge_infos.right_edge = right_edge;
    // edge_infos.left_edge = left_edge;

    if (raw_edge_sensors == 1) {
      edge_infos.right_edge = right_edge;
      edge_infos.left_edge = left_edge;
    } else {
      edge_infos.right_edge = right_edge < right_edge_limit ? true : false;
      edge_infos.left_edge =  left_edge < left_edge_limit ? true : false;
    }

    // ESP_LOGD(TAG, "RE: %d    LE: %d", right_edge, left_edge);
    xQueueSend(edge_queue, &edge_infos, portMAX_DELAY);
    // vTaskDelay(1);
  }
}
