#ifndef __BLUETOOTH_TASK_H__
#define __BLUETOOTH_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t bluetooth_task_handle;

void bluetooth_task(void *pvParameters);

#endif // __BLUETOOTH_TASK_H__