#ifndef __MPU_TASK_H__
#define __MPU_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

typedef struct {
  int16_t aclX;
  int16_t aclY;
  int16_t aclZ;
  int16_t angX;
  int16_t angY;
  int16_t angZ;
} mpu_infos_t;

extern TaskHandle_t mpu_task_handle;

extern QueueHandle_t mpu_queue;

extern QueueHandle_t mpu_calibrate_queue;

void mpu_task(void *pvParameters);

#endif // __MPU_TASK_H__