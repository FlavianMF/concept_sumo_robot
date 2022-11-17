#include <Arduino.h>

#include "logger.h"
#include "my_eeprom.h"
#include "setup_tasks.h"
#include "motors.h"
// #include "vl_sensors.h"
// #include "edge_sensors.h"

// static const char *TAG = "SETUP";

void setup() {
  setup_logger();
  ESP_LOGV(TAG, "Init setup");

  setup_motors();
  // setup_vl_sensors();
  // setup_edge_sensors();

  drive_motors(0,0);

  setup_tasks();

  initialize_variables();
}

void loop() {
  vTaskDelete(NULL);
}