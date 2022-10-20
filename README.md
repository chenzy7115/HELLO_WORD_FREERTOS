# freeRTOS变量、函数命名规范

/**********变量**********/

1.c ： Char

2.s ： int16_t，short

3.l ： int32_t，long

4.x ：BaseType_t，和其他非标准的类型：结构体、task handle、queue handle等

5.u ：unsigned

6.p ： 指针

7.uc ： uint8_t，unsigned char

8.pc ：char指针


/**********函数**********/

9.vTaskPrioritySet 返回值类型：void 在task.c中定义

10.xQueueReceive 返回值类型：BaseType_t 在queue.c中定义

11.pvTimerGetTimerID 返回值类型：pointer to void在timer.c中定义

/**********宏**********/

12.port (比如portMAX_DELAY) ： 在portable.h或portmacro.h

13.task (比如taskENTER_CRITICAL()) ： task.h

14.pd (比如pdTRUE)：  projdefs.h

15.config (比如configUSE_PREEMPTION) ： FreeRTOSConfig.h

16.err (比如errQUEUE_FULL) ：projdefs.h

/**********常用宏**********/

pdTRUE=1 ； pdFALSE=0 ； pdPASS=1 ； pdFAIL=0


# 常用函数：
1.获得函数优先级的函数 UBaseType_t uxTaskPriorityGet( TaskHandle_t pxTask );


# 使用xTaskCreate()进行参数设置时如果分配空间为1024会发生栈溢出（stack overflow）的情况; 

目前发现只在ESP32中发生，至少可以确定的是ESP32C3不会出现问题。解决办法是增加分配给task的内存空间，具体需要增加多少没有进行测试。code：

xTaskCreate(myTask, "myTask1", 2048, (void *) &xStructTest, 1, NULL);

# Soc：ESP32   IDF:VSCODE   OS:freeRTOS   framwork:ESP32-IDF

by chenzy
2022-10-20


