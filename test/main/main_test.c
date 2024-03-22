#include <stdio.h>
#include "mytask_list.h"

static void core_task()
{
    while (1)
    {
        printf("%d\n", xPortGetCoreID());
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}
void app_main(void)
{
    // xTaskCreate(single_task1, "singel_task1", 1024, (void *)&LED1, 1, NULL);
    // xTaskCreate(single_task2, "singel_task2", 1024, (void *)&LED2, 1, NULL);
    // xTaskCreate(mult_task, "mult_task1", 1024, (void *)&led1, 1, NULL);
    // xTaskCreate(mult_task, "mult_task2", 1024, (void *)&led2, 1, NULL);

    xTaskCreatePinnedToCore(core_task, "core_task", 1024 * 4, NULL, 1, NULL, 0);

    while (1)
    {
        // // printf("t1=%d  t2=%d\n", LED1, LED2);
        // // printf("led1=%d led2=%d\n", led1.pin, led2.pin);
        task_list();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    vTaskDelete(NULL);
}