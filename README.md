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

pdTRUE=1 ； pdFALSE=0 ； pdPASS=1 ； pdFAIL=0 ; pdMS_TO_TICKS(1000):毫秒转为秒


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

7./***查看Task列表 ：***/用于调试时了解系统运行的情况，对系统资源使用量较大，并且不存在中断。使用时要对freeRTOS进行配置，默认不能使用。具体查看freeRTOS参考手册的说明

include “FreeRTOS.h”

include “task.h”

void vTaskList( char *pcWriteBuffer );

/***State***/ – The state of the task at the time vTaskList() was called, as follows:

o ‘X’ if the task is executing (the task that called vTaskList()).执行

o ‘B’ if the task is in the Blocked state.阻塞

o ‘R’ if the task is in the Ready state.准备

o ‘S’ if the task is in the Suspended state, or in the Blocked state without a 
timeout.挂起（暂停）

o ‘D’ if the task has been deleted, but the idle task has not yet freed the memory 
that was being used by the task to hold its data structures and stack.删除

8./***查看task剩余的stack的空间大小 ：***/用于查看creat创建的stack是否够用

include “FreeRTOS.h”

include “task.h”

UBaseType_t uxTaskGetStackHighWaterMark( TaskHandle_t xTask );

# watch dog的使用

1.watch dog有两种，一种是中断的watch dog，一种是task的 watch dog.

2.中断watch dog里面不能执行太多的代码，从而导致其他任务没有时间执行

3.esp_task_wdt_add(句柄) 开启看门口
 
  esp_task_wdt_reset(句柄) 喂狗
  
# 队列

/***创建一个新的队列***/

include “FreeRTOS.h”

include “queue.h”

QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );

/***发送数据TO队列***/xQueueSend（）和 xQueueSendToBack（）的结果相同都是发送数据到队列的尾部，xQueueSendToFront（）发送到队列头

include “FreeRTOS.h”

include “queue.h”

BaseType_t xQueueSend( QueueHandle_t xQueue, 
 const void * pvItemToQueue, 
 TickType_t xTicksToWait );

BaseType_t xQueueSendToFront( QueueHandle_t xQueue, 
 const void * pvItemToQueue, 
 TickType_t xTicksToWait );

BaseType_t xQueueSendToBack( QueueHandle_t xQueue, 
 const void * pvItemToQueue, 
 TickType_t xTicksToWait );
 
 /***接收数据from队列***/

include “FreeRTOS.h”

include “queue.h”

BaseType_t xQueueReceive( QueueHandle_t xQueue, 
 void *pvBuffer, 
 TickType_t xTicksToWait );

 /***获得队列中item的个数***/
 
include “FreeRTOS.h”

include “queue.h”

UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue );

 /***创建队列集合(QUEUE SET)***/

include “FreeRTOS.h”

include “queue.h”

QueueSetHandle_t xQueueCreateSet( const UBaseType_t uxEventQueueLength );

 /***加入队列集合(ADD SET)***/

include “FreeRTOS.h”

include “queue.h”

BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
 QueueSetHandle_t xQueueSet );

 /***选择从队列集合中获得数据(select from set)***/

include “FreeRTOS.h”

include “queue.h”

QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
 const TickType_t xTicksToWait );

 /***重写队列里所有数据(over write)***/

include “FreeRTOS.h”

include “queue.h”

BaseType_t xQueueOverwrite( QueueHandle_t xQueue, const void *pvItemToQueue );

/***获取队列里数据但不会删除队列里的数据(peek)***/

include “FreeRTOS.h”

include “queue.h”

BaseType_t xQueuePeek( QueueHandle_t xQueue, 
 void *pvBuffer, TickType_t
 xTicksToWait );

 # RTOS的软件定时器：1.同平台无关；2.同硬件的定时器数量无关

 /***创建一个软件定时器***/
 
include “FreeRTOS.h”

include “timers.h”

TimerHandle_t xTimerCreate( const char *pcTimerName, 
 const TickType_t xTimerPeriod, 
 const UBaseType_t uxAutoReload, 
 void * const pvTimerID, 
 TimerCallbackFunction_t pxCallbackFunction );

 /***创建一个软件定时器***/

include “FreeRTOS.h”

include “timers.h”

BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait );

 /***打开/关闭一个软件定时器***/xTimerStart()打开，xTimerStop()关闭

include “FreeRTOS.h”

include “timers.h”

BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait );

BaseType_t xTimerStop( TimerHandle_t xTimer, TickType_t xTicksToWait );

 /***获得定时器的name***/

include “FreeRTOS.h”

include “timers.h”

const char * pcTimerGetName( TimerHandle_t xTimer );

/***获得定时器的ID***/

include “FreeRTOS.h”

include “timers.h”

void *pvTimerGetTimerID( TimerHandle_t xTimer );

/***Reset Timer***/

include “FreeRTOS.h”

include “timers.h”

BaseType_t xTimerReset( TimerHandle_t xTimer, TickType_t xTicksToWait );

/***改变定时运行周期(Period:周期)***/

include “FreeRTOS.h”

include “timers.h”

BaseType_t xTimerChangePeriod( TimerHandle_t xTimer, 
 TickType_t xNewPeriod,
 TickType_t xTicksToWait );

 # 二进制信号量

 /***二进制信号量的创建Create binary semephore***/

include “FreeRTOS.h”

include “semphr.h”

SemaphoreHandle_t xSemaphoreCreateBinary( void );

/***释放（GIVE）信号***/信号量被创建后先释放才可以使用，并且在使用信号量后也需要释放后才可以使用

include “FreeRTOS.h”

include “semphr.h”

BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore );

/***获取（TAKE）信号量***/

include “FreeRTOS.h”

include “semphr.h”

BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait );

#计数性信号量

/***创建一个计数型信号量***/

include “FreeRTOS.h”

include “semphr.h”

SemaphoreHandle_t xSemaphoreCreateCounting( UBaseType_t uxMaxCount, 
 UBaseType_t uxInitialCount );

/***获取信号量的计数器GET COUNT***/

include “FreeRTOS.h”

include “semphr.h”

UBaseType_t uxSemaphoreGetCount( SemaphoreHandle_t xSemaphore );

# 使用xTaskCreate()进行参数设置时如果分配空间为1024会发生栈溢出（stack overflow）的情况; 

目前发现只在ESP32中发生，至少可以确定的是ESP32C3不会出现问题。解决办法是增加分配给task的内存空间，具体需要增加多少没有进行测试。code：

xTaskCreate(myTask, "myTask1", 2048, (void *) &xStructTest, 1, NULL);

# Soc：ESP32 ;  IDF:VSCODE ;  OS:freeRTOS  ;  framwork:ESP32-IDF

by chenzy
2022-10-20


