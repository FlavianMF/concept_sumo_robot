#ifndef __IR_TASK_H__
#define __IR_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t ir_task_handle;

extern QueueHandle_t ir_queue;

void ir_task(void *pvParameters);
#endif // __IR_TASK_H__