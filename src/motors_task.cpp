#include "tasks/motors_task.h"

#include <Arduino.h>
#include "esp32-hal-log.h"
#include "motors.h"
#include "move_functions.h"

static const char *TAG = "motors_task";

TaskHandle_t motors_task_handle;

QueueHandle_t motors_queue;

float pwm = 0;
float linear_time = 0;

void motors_task(void *pvParameters) {
  motors_queue = xQueueCreate(1, sizeof(motors_functions_t));

  setup_motors();

  stop_motors();

  motors_functions_t function = STOPED;
  // uint64_t timer = 0;
  while(true) {
    xQueueReceive(motors_queue, &function, portMAX_DELAY);

    // timer = millis();
    switch(function) {
      case FORWARD: 
        ESP_LOGD(TAG, "Forward fuction received");
        forward_move(pwm, linear_time);
        break;
      default:
        ESP_LOGE(TAG, "Error with motor function");
        break;
    }

    vTaskDelay(1);
  }
}
