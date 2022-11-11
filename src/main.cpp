#include <Arduino.h>

#include "logger.h"
#include "my_eeprom.h"
#include "setup_tasks.h"

static const char *TAG = "SETUP";

void setup() {
  setup_logger();
  ESP_LOGV(TAG, "Init setup");

  setup_tasks();

  initialize_variables();
}

void loop() {
  vTaskDelete(NULL);
}