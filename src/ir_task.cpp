#include "tasks/ir_task.h"

#include "ir_receiver.h"
#include "setup_tasks.h"
#include "tasks/rgb_task.h"
#include "tasks/fight_task.h"
#include "tasks/bluetooth_task.h"

static const char *TAG = "ir_task";

TaskHandle_t ir_task_handle;

QueueHandle_t ir_queue;

static bool can_fight = false;

static uint64_t ir_message = 0;

void ir_task(void *pvParameters) {
  ESP_LOGV(TAG, "ir_task");

  ir_queue = xQueueCreate(1, sizeof(bool));

  xSemaphoreTake(setup_mutex, portMAX_DELAY);
  setup_ir_receiver();
  xSemaphoreGive(setup_mutex);

  rgb_state_t rgb_state;

  while (true) {
    ir_message = read_ir();

    // ESP_LOGD(TAG, "ir message: %ld", ir_message);

    if (ir_message == PREPARE) {
      rgb_state = PREPARED;
      xQueueSend(rgb_state_queue, &rgb_state, 1);
    } else if (ir_message == GO) {
      if (can_fight) {
        rgb_state = IDLE;
        xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
        xTaskCreatePinnedToCore(fight_task, "fight", 2048, NULL, 1, &fight_task_handle, 0);
      }
    } else if (ir_message == STOP) {
      rgb_state = STATUS_OK;
      xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
      xTaskCreatePinnedToCore(bluetooth_task, "bluetooth", 4096, NULL, 1, &bluetooth_task_handle, 0);
      can_fight = false;
    }

    xQueueReceive(ir_queue, &can_fight, 1);
    // vTaskDelay(1);
  }
}