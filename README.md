# 数据类型

每个移植的版本都含有自己的 portmacro.h 头文件，里面定义了2个数据类型：

/***TickType_t：***/

FreeRTOS配置了一个周期性的时钟中断：Tick Interrupt

每发生一次中断，中断次数累加，这被称为tick count

tick count这个变量的类型就是TickType_t

TickType_t可以是16位的，也可以是32位的

FreeRTOSConfig.h中定义configUSE_16_BIT_TICKS时，TickType_t就是uint16_t

否则TickType_t就是uint32_t

对于32位架构，建议把TickType_t配置为uint32_t

/***BaseType_t：***/

这是该架构最高效的数据类型

32位架构中，它就是uint32_t

16位架构中，它就是uint16_t

8位架构中，它就是uint8_t

BaseType_t通常用作简单的返回值的类型，还有逻辑值，比如 pdTRUE/pdFALSE

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
1./***获得函数优先级的函数 ：***/  

  include “FreeRTOS.h”
  
  include “task.h”

  UBaseType_t uxTaskPriorityGet( TaskHandle_t pxTask );

默认最大优先级为24（0-24）共25个,如果设置值超过/**24**/则编译器会改为24

2./***修改函数优先级 ：***/  
  
  include “FreeRTOS.h”
  
  include “task.h”
  
  void vTaskPrioritySet( TaskHandle_t pxTask, UBaseType_t uxNewPriority );
  
3./***挂起任务task ：***/  

include “FreeRTOS.h”

include “task.h”

void vTaskSuspend( TaskHandle_t pxTaskToSuspend );

4./***恢复任务task ：***/ 

include “FreeRTOS.h”

include “task.h”

void vTaskResume( TaskHandle_t pxTaskToResume );

5./***挂起任务调度器，在被挂起后不能运行freeRTOS的API function ：***/ 
Other FreeRTOS API functions must not be called while the scheduler is suspended.

include “FreeRTOS.h”

include “task.h”

void vTaskSuspendAll( void );

6./***恢复任务调度器 ：***/ 

include “FreeRTOS.h”

include “task.h”

BaseType_t xTaskResumeAll( void );

7./***查看Task列表 ：***/用于调试时了解系统运行的情况

include “FreeRTOS.h”

include “task.h”

void vTaskList( char *pcWriteBuffer );


# 使用xTaskCreate()进行参数设置时如果分配空间为1024会发生栈溢出（stack overflow）的情况; 

目前发现只在ESP32中发生，至少可以确定的是ESP32C3不会出现问题。解决办法是增加分配给task的内存空间，具体需要增加多少没有进行测试。code：

xTaskCreate(myTask, "myTask1", 2048, (void *) &xStructTest, 1, NULL);

# Soc：ESP32 ;  IDF:VSCODE ;  OS:freeRTOS  ;  framwork:ESP32-IDF

by chenzy
2022-10-20


