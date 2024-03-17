#include"mytask_list.h"

void task_list(void)
{
    char ptrTaskList[250];
    vTaskList(ptrTaskList);
    printf("*******************************************\n");
    printf("Task            State   Prio    Stack    Num\n");
    printf("*******************************************\n");
    printf(ptrTaskList);
    printf("*******************************************\n");
}