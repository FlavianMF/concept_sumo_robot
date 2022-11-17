#include "vl_sensors.h"

#include "esp32-hal-log.h"
#include <Arduino.h>
#include "tasks/rgb_task.h"

static const char *TAG = "vl_sensors";

Adafruit_VL53L0X front_sensor;
VL53L0X_RangingMeasurementData_t front_sensor_measure;

void setup_vl_sensors() {
  pinMode(front_xshut_pin, OUTPUT);

  digitalWrite(front_xshut_pin, LOW);
  vTaskDelay(pdMS_TO_TICKS(10));

  digitalWrite(front_xshut_pin, HIGH);
  vTaskDelay(pdMS_TO_TICKS(10));

  digitalWrite(front_xshut_pin, LOW);

  digitalWrite(front_xshut_pin, HIGH);
  delay(10);
  if (!front_sensor.begin(front_sensor_id, true, &Wire, Adafruit_VL53L0X::VL53L0X_SENSE_HIGH_SPEED)) {
    ESP_LOGE(TAG, "Front sensor are not initialized");
    rgb_state_t rgb_state = ANY_ERROR;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  } else {
    ESP_LOGI(TAG, "Front sensor are initialized");
    rgb_state_t rgb_state = STATUS_OK;
    xQueueSend(rgb_state_queue, &rgb_state, portMAX_DELAY);
  }
}

int read_front_sensor() {
  while(true) {
    if (!front_sensor.rangingTest(&front_sensor_measure, false)) {
      return front_sensor_measure.RangeStatus == 0 ? front_sensor_measure.RangeMilliMeter : 10000;
    }
    vTaskDelay(1);
    // vTaskDelay(pdMS_TO_TICKS(1));
  }
}
