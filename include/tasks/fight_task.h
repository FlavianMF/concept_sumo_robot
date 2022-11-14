#ifndef __FIGHT_TASK_H__
#define __FIGHT_TASK_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t fight_task_handle;

typedef void (*fight_function_t)();

extern fight_function_t output;
extern fight_function_t search;

void fight_task(void *pvParameters);
#endif // __FIGHT_TASK_H__