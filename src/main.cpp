#include <Arduino.h>

#include "logger.h"
#include "setup_tasks.h"

static const char *TAG = "SETUP";

void setup() {
  setup_logger();
  ESP_LOGV(TAG, "Init setup");

  setup_tasks();
}

void loop() {
  vTaskDelete(NULL);
}