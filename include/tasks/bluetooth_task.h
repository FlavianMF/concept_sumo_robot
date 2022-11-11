#ifndef __BLUETOOTH_TASK_H__
#define __BLUETOOTH_TASK_H__
#include <Arduino.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t bluetooth_task_handle;

extern float bluetooth_pwm;

void bluetooth_task(void *pvParameters);

void execute_bluetooth_command(char *message);

#endif // __BLUETOOTH_TASK_H__