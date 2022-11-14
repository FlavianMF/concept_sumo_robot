#include "edge_sensors.h"

#include <Arduino.h>
#include <esp32-hal-log.h>

static const char *TAG = "edge_sensors";

void setup_edge_sensors() {
  ESP_LOGV(TAG, "setup_edge_sensors");
  pinMode(right_edge_pin, INPUT);
  pinMode(left_edge_pin, INPUT);

  // Serial.println("[SETUP] Edges Init");
  // bluetooth_serial.println("[SETUP] Edges Init");
}