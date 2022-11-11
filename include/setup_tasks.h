#ifndef __SETUP_TASKS_H__
#define __SETUP_TASKS_H__

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

extern SemaphoreHandle_t setup_mutex;

void setup_tasks(void);

#endif // __SETUP_TASKS_H__