#include "single_paramenters.h"

int LED1 = 0, LED2 = 0;

void single_task1(void *ptr)
{
    int *PLED;
    PLED = (int *)ptr;

    while (1)
    {
        (*PLED)++;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void single_task2(void *ptr)
{
    int *LEDPIN = (int *)ptr;
    while (1)
    {
        (*LEDPIN) += 3;
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
    vTaskDelete(NULL);
}
