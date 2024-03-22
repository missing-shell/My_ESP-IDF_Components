#include "mult_paramenters.h"

LEDFLASH led1 = {0, 1000};
LEDFLASH led2 = {0, 3000};

void mult_task(void *ptr)
{
    LEDFLASH *pledflash = (LEDFLASH *)ptr;
    int *pin = &pledflash->pin;
    int delaytime = pledflash->delaytime;

    while (1)
    {
        if (ptr == &led1)
        {
            (*pin)++;
        }
        else if (ptr == &led2)
        {
            (*pin) += 3;
        }

        vTaskDelay(delaytime / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}