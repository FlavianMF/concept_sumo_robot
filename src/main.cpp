#include <Arduino.h>

#include "logger.h"

static const char *TAG = "ESP";

void setup() {
  setup_logger();
  ESP_LOGI(TAG, "Test log info");
}

void loop() {
  ESP_LOGV(TAG, "The time of the millis() is: %li", millis());
  vTaskDelay(pdMS_TO_TICKS(5000));
}