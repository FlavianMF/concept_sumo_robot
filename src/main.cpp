#include <Arduino.h>

#include "logger.h"
#include "rgb.h"
#include "bluetooth.h"
// #include "mpu.h"
#include "setup_tasks.h"

static const char *TAG = "SETUP";

void setup() {
  setup_logger();
  ESP_LOGV(TAG, "Init setup");
  rgb.setColor(RGBLed::YELLOW);

  setup_bluetooth();
  // setup_mpu();

  setup_tasks();
}

void loop() {
  ESP_LOGV(TAG, "The time of the millis() is: %li", millis());
  vTaskDelay(pdMS_TO_TICKS(5000));
}