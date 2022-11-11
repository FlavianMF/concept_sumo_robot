#include "tasks/rgb_task.h"

#include "esp32-hal-log.h"
#include "rgb.h"

static const char *TAG = "rgb_task";

TaskHandle_t rgb_task_handle;

QueueHandle_t rgb_state_queue;

void rgb_task(void *pvParameters) {
  ESP_LOGV(TAG, "rgb task init");

  rgb_state_queue = xQueueCreate(1, sizeof(rgb_state_t));

  rgb.setColor(RGBLed::YELLOW);

  rgb_state_t rgb_state = PASS;

  uint64_t flash_time = 0;

  while(true) {
    xQueueReceive(rgb_state_queue, &rgb_state, 1);

    switch(rgb_state) {
      case IDLE:
        rgb_idle();
        rgb_state = PASS;
        break;
      case ANY_ERROR:
        rgb_any_error(&flash_time);
        break;
      case STATUS_OK:
        rgb_successful();
        rgb_state = PASS;
        break;
      case PASS:
        break;
      default:
        ESP_LOGW(TAG, "Invalid state: %d", rgb_state);
        break;
    }
  }
}
