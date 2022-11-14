#ifndef __MOTORS_TASK_H__
#define __MOTORS_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

extern TaskHandle_t motors_task_handle;

extern QueueHandle_t motors_queue;

typedef enum {
  FORWARD,
  BACKWARD,
  LEFTWARD,
  RIGHTWARD,
  STOPED,
} motors_functions_t;

extern float pwm;
extern float linear_time;

void motors_task(void *pvParameters);

#endif // __MOTORS_TASK_H__