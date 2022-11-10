#ifndef __MPU_TASK_H__
#define __MPU_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t mpu_task_handle;

extern QueueHandle_t mpu_queue;

void mpu_task(void *pvParameters);

#endif // __MPU_TASK_H__