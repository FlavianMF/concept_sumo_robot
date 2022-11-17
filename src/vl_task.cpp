#include "tasks/vl_task.h"

#include "vl_sensors.h"
#include "esp32-hal-log.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"

TaskHandle_t vl_task_handle;

QueueHandle_t vl_queue;

static const char *TAG = "vl_task";

void vl_task(void *pvParameters) {
  ESP_LOGV(TAG, "vl_task");


  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  vl_queue = xQueueCreate(1, sizeof(vl_readings_t));
  if (vl_queue == NULL) {
    ESP_LOGE(TAG, "Queue not created");
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  }
  setup_vl_sensors();
  xSemaphoreGive(setup_mutex);

  vl_readings_t vl_reading;

  // vTaskSuspend(NULL);
  while(true) {

    vl_reading.front_reading = read_front_sensor();

    xQueueSend(vl_queue, &vl_reading, portMAX_DELAY);
    
    // vTaskDelay(1);
  }
}
