#ifndef MULT_PARAMENTERS_H
#define MULT_PARAMENTERS_H

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

typedef struct
{
    int pin;
    int delaytime;
} LEDFLASH;


extern LEDFLASH led1, led2;

void mult_task(void *ptr);
 
#endif