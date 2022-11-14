#ifndef __VL_TASK_H__
#define __VL_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t vl_task_handle;

extern QueueHandle_t vl_queue;

typedef struct {
  int front_reading;
  int rear_reading;
  int left_reading;
  int right_reading;
} vl_readings_t;

void vl_task(void *pvParameters);

#endif // __VL_TASK_H__