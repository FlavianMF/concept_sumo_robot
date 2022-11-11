#ifndef __RGB_TASK_H__
#define __RGB_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t rgb_task_handle;

extern QueueHandle_t rgb_state_queue;

typedef enum {
  IDLE,
  PASS,
  ANY_ERROR,
  STATUS_OK,
} rgb_state_t;

void rgb_task(void *pvParameters);

#endif // __RGB_TASK_H__