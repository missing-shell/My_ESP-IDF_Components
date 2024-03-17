#ifndef SINGLE_PARAMENTERS_H
#define SINGLE_PARAMENTERS_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

extern int LED1,LED2;

void single_task1(void *ptr);
void single_task2(void *ptr);

#endif