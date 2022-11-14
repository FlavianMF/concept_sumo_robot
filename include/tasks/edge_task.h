#ifndef __EDGE_TASK_H__
#define __EDGE_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t edge_task_handle;
extern QueueHandle_t edge_queue;

typedef struct {
  int right_edge;
  int left_edge;
} edge_infos_t;

extern float right_edge_limit;
extern float left_edge_limit;

void edge_task(void *pvParameters);
#endif // __EDGE_TASK_H__