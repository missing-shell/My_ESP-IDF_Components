# 组件结构

```tree
-
task_list
       ├── test
       |     ├── main
       |     |    └── main_test.c
       |     |    └── CMakeLists.txt
       |     └── CMakeLists.txt
       ├── CMakeLists.txt
       ├── include
       │   └── mult_paramenters.h
       │   └── mytask_list.h
       |   └── single_paramenters.h
       └── src
            └── mult_paramenters.c
            └── mytask_list.c
            └── single_paramenters.c
```

## 初始设置

ESP32可以通过串口查看各个任务的内存使用情况。这通常通过使用FreeRTOS的内存管理功能来实现。FreeRTOS提供了一些API，如`xPortGetFreeHeapSize()`，可以用来获取当前可用的堆内存大小，但这并不能直接提供每个任务的内存使用情况。

要查看每个任务的内存使用情况，你需要使用FreeRTOS的任务栈监视器（Task Stack Monitor）功能。这个功能可以在编译时启用，并且可以通过串口输出每个任务的栈使用情况。

以下是如何在ESP-IDF中启用任务栈监视器的步骤：

- 修改项目配置：

    在项目的`sdkconfig`文件中，找到`CONFIG_FREERTOS_USE_TRACE_FACILITY`选项，确保它被设置为`y`。这将启用FreeRTOS的跟踪功能。

    勾选下面三个选项

    ```markdown
    configUSE_TRACE_FACILITY

    configUSE_STATS_FORMATTING_FUNCTIONS

    Enable display of xCoreID in vTaskList
    ```

请注意，这些步骤可能会根据你使用的ESP-IDF版本有所不同。如果你使用的是旧版本的ESP-IDF，可能需要查阅相应版本的文档来找到正确的配置选项。

此外，任务栈监视器提供的信息是每个任务的栈使用情况，而不是堆内存使用情况。如果你需要查看堆内存使用情况，可以使用`xPortGetFreeHeapSize()`函数，但这需要你自己编写代码来输出结果。
