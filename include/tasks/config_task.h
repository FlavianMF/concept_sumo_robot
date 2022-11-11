#ifndef __CONFIG_TASK_H__
#define __CONFIG_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t config_task_handle;

void config_task(void *pvParameters);

#endif // __CONFIG_TASK_H__