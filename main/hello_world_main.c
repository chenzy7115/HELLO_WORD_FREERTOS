/*
test API of freeRTOS.
Soc:ESP32
1.教训视频：
https://space.bilibili.com/1338335828/channel/collectiondetail?sid=79734
2.README包含freeRTOS主要函数的介绍
3.从start to end是一个测试模块，需要单独使用，h文件不修改
*/
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "freertos/stream_buffer.h"
#include "freertos/message_buffer.h"

// -----------------------------START MESSSAGE BUFF TWO--------------------------------------------

MessageBufferHandle_t MessageBufferHandle = NULL;

void Task1(void *pvParam)
{
   
    int str_len;
    int send_bytes = 0;

    char tx_buff[50];

    for (int i=0 ; i<3 ; i++)
    {
        str_len = sprintf(tx_buff, "Hello , I am Chenzy's NO.: %d", i); //初始字符串函数，并返回字符串长度
        send_bytes = xMessageBufferSend(MessageBufferHandle,
                                       (void *)tx_buff,
                                       str_len,
                                       portMAX_DELAY);
        printf("---------------------\n");
        printf("Send i = %d ,send_bytes = %d\n", i, send_bytes);

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
    vTaskDelete(NULL);
}

void Task2(void *pvParam)
{
    char rx_buff[200];
    int rec_bytes = 0;
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        memset(rx_buff, 0, sizeof(rx_buff)); //初始化接收数据的buff，将值都置零

        rec_bytes = xMessageBufferReceive(MessageBufferHandle,
                             (void *)rx_buff,
                             sizeof(rx_buff),
                             portMAX_DELAY);
        printf("---------------------\n");
        printf("Reseive rec_bytes = %d ,data = %s\n", rec_bytes, rx_buff);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    MessageBufferHandle = xMessageBufferCreate(1000); // BUFF的大小为1000字节
    if (MessageBufferHandle != NULL)
    {
        xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
        xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 1, NULL);
    }
    else
    {
        printf("---------------------\n");
        printf("StreamBUFF fail !\n");
    }
}

// ------------------------------------END MESSSAGE BUFF TWO-------------------------------------

// // -------------------------START队列的多进单出--------------------------------------------------
// void sendTasc1(void *pvParam)
// {
//     QueueHandle_t QHandle;
//     QHandle = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i = 111;
//     while (1)
//     {
//         xStatus = xQueueSend(QHandle, &i, 0);
//         if (xStatus != pdPASS)
//             printf("Send1 fail!\n");
//         else
//             printf("Send1 done!\n");
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void sendTasc2(void *pvParam)
// {
//     QueueHandle_t QHandle;
//     QHandle = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i = 222;
//     while (1)
//     {
//         xStatus = xQueueSend(QHandle, &i, 0);
//         if (xStatus != pdPASS)
//             printf("Send2 fail!\n");
//         else
//             printf("Send2 done!\n");
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void recTasc(void *pvParam)
// {
//     QueueHandle_t QHandle;
//     QHandle = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i = 0;
//     while (1)
//     {

//         xStatus = xQueueReceive(QHandle, &i, portMAX_DELAY); // portMAX_DELAY可以无限delay，未接收到数据时阻塞receive返回数据直到接收到数据时
//         if (xStatus != pdPASS)
//             printf("Receive fail!\n");
//         else
//             printf("Receive message:%d\n", i);

//     }
// }

// void app_main(void)
// {
//     // TaskHandle_t pxTask1;
//     QueueHandle_t QHandle;
//     QHandle = xQueueCreate(5, sizeof(int));

//     if (QHandle != NULL)
//     {
//         printf("Create queue successfully!\n");
//         xTaskCreate(sendTasc1, "sendTasc", 1024 * 5, (void *)QHandle, 1, NULL);
//         xTaskCreate(sendTasc2, "sendTasc", 1024 * 5, (void *)QHandle, 1, NULL);
//         xTaskCreate(recTasc, "recTasc", 1024 * 5, (void *)QHandle, 2, NULL);
//     }
//     else
//     {
//         printf("Can't create queue!\n");
//     }
// }
// // ---------------------------END队列的多进单出-------------------------------------------

// // -----------------------------START队列集合（QUEUE SET)--------------------------------------------
// void sendTasc1(void *pvParam)
// {
//     QueueHandle_t QHandle;
//     QHandle = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i = 111;
//     while (1)
//     {
//         xStatus = xQueueSend(QHandle, &i, 0);
//         if (xStatus != pdPASS)
//             printf("Send1 fail!\n");
//         else
//             printf("Send1 done!\n");
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void sendTasc2(void *pvParam)
// {
//     QueueHandle_t QHandle;
//     QHandle = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i = 222;
//     while (1)
//     {
//         xStatus = xQueueSend(QHandle, &i, 0);
//         if (xStatus != pdPASS)
//             printf("Send2 fail!\n");
//         else
//             printf("Send2 done!\n");
//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void recTasc(void *pvParam)
// {
//     QueueSetHandle_t QueueSet;
//     QueueSet = (QueueSetHandle_t)pvParam;

//     QueueSetMemberHandle_t  QueueDate;

//     BaseType_t xStatus;

//     int i = 0;
//     while (1)
//     {
//         QueueDate = xQueueSelectFromSet(QueueSet,portMAX_DELAY);
//         xStatus = xQueueReceive(QueueDate, &i, portMAX_DELAY); // portMAX_DELAY可以无限delay，未接收到数据时阻塞receive返回数据直到接收到数据时
//         if (xStatus != pdPASS)
//             printf("Receive fail!\n");
//         else
//             printf("Receive message:%d\n", i);
//     }
// }

// void app_main(void)
// {
//     // TaskHandle_t pxTask1;
//     QueueHandle_t QHandle1;
//     QHandle1 = xQueueCreate(5, sizeof(int));

//     QueueHandle_t QHandle2;
//     QHandle2 = xQueueCreate(5, sizeof(int));

//     QueueSetHandle_t QueueSet;
//     QueueSet = xQueueCreateSet(10); // queue set的创建长度应该等于需要加入的quene长度的和

//     xQueueAddToSet(QHandle1, QueueSet); //第一个队列加入SET
//     xQueueAddToSet(QHandle2, QueueSet); //第二个队列加入SET

//     if ((QHandle1 != NULL) && (QHandle2 != NULL) && (QueueSet != NULL)) //只有队列1、2和队列集合都create时
//     {
//         printf("Create queue successfully!\n");
//         xTaskCreate(sendTasc1, "sendTasc", 1024 * 5, (void *)QHandle1, 1, NULL);
//         xTaskCreate(sendTasc2, "sendTasc", 1024 * 5, (void *)QHandle2, 1, NULL);
//         xTaskCreate(recTasc, "recTasc", 1024 * 5, (void *)QueueSet, 2, NULL); //获得QueueSet句柄
//     }
//     else
//     {
//         printf("Can't create queue!\n");
//     }
// }
//// ------------------------------------END队列集合（MAIL BOX)---------------------------------------

// // -----------------------------START队列邮箱（QUEUE MAILBOX)--------------------------------------------
// // queue mailbox里的数据会hold data,The data been can read with anyTask，but the data be don't change。
// // mailbox里只有一个队列,本质就是一个queue但是数据个数只有1
// void writeTasc(void *pvParam)
// {
//     QueueHandle_t MailBox;
//     MailBox = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i = 0;
//     while (1)
//     {
//         xStatus = xQueueOverwrite(MailBox, &i);
//         if (xStatus != pdPASS)
//             printf("Write MailBox fail!\n");
//         else
//             printf("Write MailBox done!\n");

//         i++;
//         vTaskDelay(6000 / portTICK_PERIOD_MS);
//     }
// }

// void readTasc(void *pvParam)
// {
//     QueueHandle_t MailBox;
//     MailBox = (QueueHandle_t)pvParam;
//     BaseType_t xStatus;

//     int i;
//     while (1)
//     {
//         xStatus = xQueuePeek(MailBox, &i, portMAX_DELAY);//protMAX_DELAY是没有读到数据就一直等待
//         if (xStatus != pdPASS)
//             printf("readTasc fail!\n");
//         else
//             printf("readTasc:%d!\n", i);

//         vTaskDelay(1000 / portTICK_PERIOD_MS);
//     }
// }

// void app_main(void)
// {
//     // TaskHandle_t pxTask1;
//     QueueHandle_t MailBox;
//     MailBox = xQueueCreate(1, sizeof(int));

//     if (MailBox != NULL)
//     {
//         printf("Create queue successfully!\n");
//         xTaskCreate(writeTasc, "writeTasc", 1024 * 5, (void *)MailBox, 1, NULL);
//         xTaskCreate(readTasc, "readTasc1", 1024 * 5, (void *)MailBox, 2, NULL);//使用1个task函数，创建3次task任务，第一次，由任务名称来确定是不同的任务
//         xTaskCreate(readTasc, "readTasc2", 1024 * 5, (void *)MailBox, 2, NULL);//使用1个task函数，创建3次task任务，第二次，由任务名称来确定是不同的任务
//         xTaskCreate(readTasc, "readTasc3", 1024 * 5, (void *)MailBox, 2, NULL);//使用1个task函数，创建3次task任务，第三次，由任务名称来确定是不同的任务
//     }
//     else
//     {
//         printf("Can't create queue!\n");
//     }
// }

//// ------------------------------------END队列邮箱（MAIL BOX)---------------------------------------

// // -----------------------------START软件定时器--------------------------------------------
// void TimerCallBack(TimerHandle_t xTimer) // time callback function的格式是固定的，除了函数名其他要和原型一致
// {
//     const char *strName;
//     int *timeID;
//     strName = pcTimerGetName(xTimer);
//     timeID = (int *)pvTimerGetTimerID(xTimer);
//     printf("Timer name : %s , ID : %d \n", strName, *timeID);
// }

// int ID1 = 1;
// int ID2 = 2;
// void app_main(void)
// {
//     TimerHandle_t xTimer1;
//     TimerHandle_t xTimer2;
//     xTimer1 = xTimerCreate("Time1", pdMS_TO_TICKS(1000), pdTRUE, (void *)&ID1, TimerCallBack);
//     xTimer2 = xTimerCreate("Time2", pdMS_TO_TICKS(2000), pdTRUE, (void *)&ID2, TimerCallBack);
//     // pdMS_TO_TICKS(1000)设置为1秒，pdFALSE不启用重载也就是只执行1次(pdTRUE就是每1秒执行一次)，(void * )0是Time ID，callback function
//     xTimerStart(xTimer1, 0);
//     xTimerStart(xTimer2, 0);

//     vTaskDelay(pdMS_TO_TICKS(6000));
//     xTimerChangePeriod(xTimer1, pdMS_TO_TICKS(6000), 0);

//     // while(1)
//     // {
//     //     vTaskDelay(pdMS_TO_TICKS(1000));
//     //     xTimerReset(xTimer2,0);
//     // }
//     //  vTaskDelay(pdMS_TO_TICKS(6000));
//     //  xTimerStop(xTimer1,0);
// }

//// ------------------------------------END软件定时器--------------------------------------

// // -----------------------------START二进制信号量（Binary Semaphore)--------------------------------------------

// int iCount = 0;
// SemaphoreHandle_t semaphrHandle;

// void myTask1(void *pvParam)
// {
//     while (1)
//     {
//         xSemaphoreTake(semaphrHandle, portMAX_DELAY); //如果没有获得信号量就无限等待
//         for (int i = 0; i < 10; i++)
//         {
//             iCount++;
//             printf("myTask1 iCount = %d\n", iCount);
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//         xSemaphoreGive(semaphrHandle); //释放信号量，可以被其他函数使用
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void myTask2(void *pvParam)
// {
//     while (1)
//     {
//         xSemaphoreTake(semaphrHandle, portMAX_DELAY); //如果没有获得信号量就无限等待
//         for (int i = 0; i < 10; i++)
//         {
//             iCount++;
//             printf("myTask2 iCount = %d\n", iCount);
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//         xSemaphoreGive(semaphrHandle); //释放信号量，可以被其他函数使用
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void app_main(void)
// {
//     semaphrHandle = xSemaphoreCreateBinary();
//     xSemaphoreGive(semaphrHandle); //创建信号量后要先释放才能被使用
//     xTaskCreate(myTask1, "myTask1", 1024 * 5, NULL, 1, NULL);
//     xTaskCreate(myTask2, "myTask2", 1024 * 5, NULL, 1, NULL);
// }

// // ------------------------------------END二进制信号量（Binary Semaphore)--------------------------------------

// // -----------------------------START计数器信号量（Count Semaphore)--------------------------------------------

// SemaphoreHandle_t semaphrHandle;

// void carInTask(void *pvParam)
// {
//     int emptySpace = 0;
//     BaseType_t iResult;
//     while (1)
//     {
//         emptySpace = uxSemaphoreGetCount(semaphrHandle);
//         printf("Empty space = %d\n", emptySpace);
//         iResult = xSemaphoreTake(semaphrHandle, 0); //获得资源，计数器-1

//         if (iResult == pdPASS)
//         {
//             printf("One car in!\n");
//         }
//         else
//         {
//             printf("No space!\n");
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void carOutTask(void *pvParam)
// {
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(6000));
//         xSemaphoreGive(semaphrHandle); //给出资源，计数器+1
//         printf("One car out!\n");
//         }
// }

// void app_main(void)
// {
//     semaphrHandle = xSemaphoreCreateCounting(5,5);//计数器最大值为5，初始计数值为5
//     xSemaphoreGive(semaphrHandle); //给出资源，计数器+1。由于计数器已经时最大值，所以计数器值不变还是5
//     xTaskCreate(carInTask, "carInTask", 1024 * 5, NULL, 1, NULL);
//     xTaskCreate(carOutTask, "carOutTask", 1024 * 5, NULL, 1, NULL);
// }

// // ------------------------------------END计数器信号量（Count Semaphore)--------------------------------------

// // -----------------------------START 互斥量(Mutex Semaphore)--------------------------------------------
// //获得mutex的task会获得被互斥的task的优先级
// SemaphoreHandle_t mutexHandle;

// void Task1(void *pvParam)
// {
//     BaseType_t iRet;
//     while (1)
//     {
//         printf("Task-1 Begin!\n");
//         iRet = xSemaphoreTake(mutexHandle,1000);
//         if (iRet == pdPASS)
//         {
//             printf("Task-1 Take!\n");
//             for (int i=0; i < 50; i++)
//             {
//                 printf("Task-1 i = %d\n",i);
//                 vTaskDelay(pdMS_TO_TICKS(1000));
//             }
//             xSemaphoreGive(mutexHandle);
//             printf("Task Give!\n");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//         else
//         {
//             printf("Task-1 didn't Take!\n");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//     }
// }

// void Task2(void *pvParam)
// {
//     printf("Task-2 Begin!\n");
//     vTaskDelay(pdMS_TO_TICKS(1000)); //阻塞一下，让优先级低的function能够运行
//     while (1)
//     {
//         ;
//     }
// }

// void Task3(void *pvParam)
// {
//     BaseType_t iRet;

//     printf("Task-3 Begin!\n");
//     vTaskDelay(pdMS_TO_TICKS(1000)); //利用pdMS_TO_TICKS延迟1000ms也就是1秒,这里时阻塞以下，让优先级低的function能够运行
//     while (1)
//     {
//         iRet = xSemaphoreTake(mutexHandle, 1000); //获得Mutex的句柄，等待1000个机器周期
//         if (iRet == pdPASS)
//         {
//             printf("Task-3 take!\n");
//             for (int i=0; i < 10; i++)
//             {
//                 printf("Task-3 i = %d\n", i);
//                 vTaskDelay(pdMS_TO_TICKS(1000));
//             }
//             xSemaphoreGive(mutexHandle);
//             printf("Task-3 Give!\n");
//             vTaskDelay(pdMS_TO_TICKS(5000));
//         }
//         else
//         {
//             printf("Task-3 didn't Take!\n");
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }
//     }
// }

// void app_main(void)
// {
//     mutexHandle = xSemaphoreCreateMutex(); // Create Mutex 并获得handle
//     // mutexHandle = xSemaphoreCreateBinary();//Create binary semaphore

//     vTaskSuspendAll(); //创建任务前挂起所有的任务调度器
//     xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
//     xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 2, NULL);
//     xTaskCreate(Task3, "Task3", 1024 * 5, NULL, 3, NULL);
//     xTaskResumeAll(); //创建任务后打开所有的任务调度器，确保任务开启顺序按优先级开始
// }

// // ------------------------------------END互斥量(Mutex Semaphore)--------------------------------------

// // -----------------------------START 递归互斥量(Recursive Mutex Semaphore)--------------------------------------------
// //获得mutex的task会获得被互斥的task的优先级
// SemaphoreHandle_t mutexHandle;

// void Task1(void *pvParam)
// {
//     while (1)
//     {
//         printf("-------------------\n");
//         printf("Task-1 Begin!\n");

//         xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
//         printf("Task-1 Take A !\n");
//         for (int i = 0; i < 10; i++)
//         {
//             printf("Task-1 i = %d for A\n", i);
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }

//         xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
//         printf("Task-1 Take B !\n");
//         for (int i = 0; i < 10; i++)
//         {
//             printf("Task-1 i %d for B\n", i);
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }

//         printf("Task-1 Give B\n");
//         xSemaphoreGiveRecursive(mutexHandle);
//         vTaskDelay(pdMS_TO_TICKS(3000));

//         printf("Task-1 Give A\n");
//         xSemaphoreGiveRecursive(mutexHandle);
//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
// }

// void Task2(void *pvParam)
// {
//     vTaskDelay(pdMS_TO_TICKS(1000)); //阻塞一下，让优先级低的function能够运行
//     while (1)
//     {
//         printf("-------------------\n");
//         printf("Task-2 Begin!\n");

//         xSemaphoreTakeRecursive(mutexHandle, portMAX_DELAY);
//         printf("Task-2 Take A !\n");
//         for (int i = 0; i < 10; i++)
//         {
//             printf("Task-2 i = %d for A\n", i);
//             vTaskDelay(pdMS_TO_TICKS(1000));
//         }

//         printf("Task-2 Give A\n");
//         xSemaphoreGiveRecursive(mutexHandle);
//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
// }

// void app_main(void)
// {
//     mutexHandle = xSemaphoreCreateRecursiveMutex(); // Create Mutex 并获得handle

//     vTaskSuspendAll(); //创建任务前挂起所有的任务调度器

//     xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
//     xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 2, NULL);

//     xTaskResumeAll(); //创建任务后打开所有的任务调度器，确保任务开启顺序按优先级开始
// }

// // ------------------------------------END 递归互斥量(Recursive Mutex Semaphore--------------------------------------

// // -----------------------------START EVENT GROUP WAIT--------------------------------------------

// EventGroupHandle_t xGreateEventGroup;

// #define BIT_0 ( 1 << 0 )//第0位
// #define BIT_4 ( 1 << 4 )//第4位

// void Task1(void *pvParam)
// {
//     //EventBits_t uxBits;
//     while (1)
//     {
//         printf("---------------------\n");
//         printf("Task-1 Begin to wait!\n");
//         xEventGroupWaitBits(
//             xGreateEventGroup,//EVENT GROUP的HANDLE
//             BIT_0 | BIT_4,//第0位按位或运算第4位得到结果0X0A（0000 1001）
//             pdTRUE,//函数结束时，对0和4位清0
//             pdTRUE,//设置为pdFALSE时，当0和4位有任何一个发生变化时就就退出BLOCK状态。设置为pdTRUE时，两个位的值都发生变化时退出BLOCK
//             portMAX_DELAY//阻塞状态等待的时长，这里设置的是无限长
//             );
//         printf("---------------------\n");
//         printf("In Task-1,BIT_0 and BIT_4 is set!\n");

//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void Task2(void *pvParam)
// {
//     vTaskDelay(pdMS_TO_TICKS(1000)); //阻塞一下，让优先级低的function能够运行
//     while (1)
//     {
//         printf("-------------------\n");
//         printf("Task-2 Begin set Bit_0!\n");

//         xEventGroupSetBits(xGreateEventGroup,BIT_0);
//         vTaskDelay(pdMS_TO_TICKS(5000));

//         printf("-------------------\n");
//         printf("Task-2 Begin set Bit_4!\n");

//         xEventGroupSetBits(xGreateEventGroup,BIT_4);
//         vTaskDelay(pdMS_TO_TICKS(5000));
//     }
// }

// void app_main(void)
// {
//     xGreateEventGroup = xEventGroupCreate();

//     if (xGreateEventGroup == NULL)
//     {
//         printf("Event Group Creata fail!\n");
//     }
//     else
//     {
//         vTaskSuspendAll();//Create Task前先暂停（suspend）任务管理器
//         xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
//         xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 2, NULL);
//         xTaskResumeAll();//Task创建后打开任务管理器，确保任务按设置的优先级执行
//     }
// }

// // ------------------------------------END EVENT GROUP WAIT--------------------------------------

// // -----------------------------START EVENT GROUP SYNC--------------------------------------------

// EventGroupHandle_t xEventBits;

// #define TASK_0_BIT (1 << 0)
// #define TASK_1_BIT (1 << 1)
// #define TASK_2_BIT (1 << 2)
// #define ALL_SYNC_BITS (TASK_0_BIT | TASK_1_BIT | TASK_2_BIT) //需要同步的位，0001 | 0010 | 0100 后的结果 0111 换为16进制 0X07

// void Task0(void *pvPavam)
// {
//     while (1)
//     {
//         printf("---------------------\n");
//         printf("Task-0 Begin to wait!\n");
//         vTaskDelay(pdMS_TO_TICKS(1000));

//         printf("---------------------\n");
//         printf("Task-0 set BIT_0!\n");
//         xEventGroupSync(xEventBits,
//                         TASK_0_BIT,    /* The bit to set. */
//                         ALL_SYNC_BITS, /* The bits to wait for. */
//                         portMAX_DELAY); /* Timeout value. */

//         printf("Task-0 Sync!\n");
//         vTaskDelay(pdMS_TO_TICKS(10000));
//     }
// }

// void Task1(void *pvParam)
// {
//     while(1)
//     {
//         printf("---------------------\n");
//         printf("Task-1 Begin to wait!\n");
//         vTaskDelay(pdMS_TO_TICKS(3000));

//         printf("---------------------\n");
//         printf("Task-1 set BIT_1!\n");
//         xEventGroupSync(xEventBits,
//                         TASK_1_BIT,    /* The bit to set. */
//                         ALL_SYNC_BITS, /* The bits to wait for. */
//                         portMAX_DELAY); /* Timeout value. */

//         printf("Task-1 Sync!\n");
//         vTaskDelay(pdMS_TO_TICKS(10000));
//     }
// }

// void Task2(void *pvParam)
// {
//     while(1)
//     {
//         printf("---------------------\n");
//         printf("Task-2 Begin to wait!\n");
//         vTaskDelay(pdMS_TO_TICKS(5000));

//         printf("---------------------\n");
//         printf("Task-2 set BIT_2!\n");
//         xEventGroupSync(xEventBits,
//                         TASK_2_BIT,    /* The bit to set. */
//                         ALL_SYNC_BITS, /* The bits to wait for. */
//                         portMAX_DELAY); /* Timeout value. */

//         printf("Task-2 Sync!\n");
//         vTaskDelay(pdMS_TO_TICKS(10000));
//     }
// }

// void app_main(void)
// {
//     xEventBits = xEventGroupCreate();

//     if (xEventBits == NULL)
//     {
//         printf("Event Group Creata fail!\n");
//     }
//     else
//     {
//         vTaskSuspendAll(); // Create Task前先暂停（suspend）任务管理器
//         xTaskCreate(Task0, "Task0", 1024 * 5, NULL, 1, NULL);
//         xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
//         xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 1, NULL);
//         xTaskResumeAll(); // Task创建后打开任务管理器，确保任务按设置的优先级执行
//     }
// }

// // // ------------------------------------END EVENT GROUP SYNC--------------------------------------

// static TaskHandle_t xTask1 = NULL;

// void Task1(void *pvParam)
// {
//     while (1)
//     {
//         printf("---------------------\n");
//         printf("Task-1 wait notification!\n");

//         ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//         printf("---------------------\n");
//         printf("Task-1 got notification!\n");
//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
// }

// void Task2(void *pvParam)
// {
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(5000));
//         printf("---------------------\n");
//         printf("Task-2 will nofitication Task1!\n");

//         // xTaskNotifyGive(Task1);
//         xTaskNotifyGive( xTask1 );
//     }
// }

// void app_main(void)
// {
//     vTaskSuspendAll(); // Create Task前先暂停（suspend）任务管理器
//     xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, &xTask1);
//     xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 1, NULL);
//     xTaskResumeAll(); // Task创建后打开任务管理器，确保任务按设置的优先级执行
// }

// // ------------------------------------END NOTIFICATION SYNC--------------------------------------

// // -----------------------------START NOTIFICATION Value--------------------------------------------

// static TaskHandle_t xTask1 = NULL;

// void Task1(void *pvParam)
// {
//     uint32_t ulNotifiedValue;
//     while (1)
//     {
//         printf("---------------------\n");
//         printf("Task-1 wait notification!\n");

//         xTaskNotifyWait(0x00,             /* Don't clear any notification bits on entry. */
//                         ULONG_MAX,        /* Reset the notification value to 0 on exit. */
//                         &ulNotifiedValue, /* Notified value pass out in ulNotifiedValue. */
//                         portMAX_DELAY);   /* Block indefinitely. */
//         /* Process any events that have been latched in the notified value. */
//         if ((ulNotifiedValue & 0x01) != 0)
//         {
//             /* Bit 0 was set - process whichever event is represented by bit 0. */
//             printf("Task-1 process BIT 0 Even!\n");
//         }
//         if ((ulNotifiedValue & 0x02) != 0)
//         {
//             /* Bit 1 was set - process whichever event is represented by bit 1. */
//             printf("Task-1 process BIT 1 Even!\n");
//         }
//         if ((ulNotifiedValue & 0x04) != 0)
//         {
//             /* Bit 2 was set - process whichever event is represented by bit 2. */
//             printf("Task-1 process BIT 4 Even!\n");
//         }

//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
// }

// void Task2(void *pvParam)
// {
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(5000));
//         printf("---------------------\n");
//         printf("Task-2 will nofitication Task1,input 0x01\n");

//         xTaskNotify( xTask1, 0x01, eSetValueWithOverwrite );
//         vTaskDelay(pdMS_TO_TICKS(5000));

//         printf("---------------------\n");
//         printf("Task-2 will nofitication Task1,input 0x02\n");
//         xTaskNotify( xTask1, 0x02, eSetValueWithOverwrite );
//         vTaskDelay(pdMS_TO_TICKS(5000));

//         printf("---------------------\n");
//         printf("Task-2 will nofitication Task1,input 0x04\n");
//         xTaskNotify( xTask1, 0x04, eSetValueWithOverwrite );
//         vTaskDelay(pdMS_TO_TICKS(5000));
//     }
// }

// void app_main(void)
// {
//     vTaskSuspendAll(); // Create Task前先暂停（suspend）任务管理器
//     xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, &xTask1);
//     xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 1, NULL);
//     xTaskResumeAll(); // Task创建后打开任务管理器，确保任务按设置的优先级执行
// }

// // ------------------------------------END NOTIFICATION Value--------------------------------------

// // -----------------------------START STream BUFF--------------------------------------------

// StreamBufferHandle_t StreamBufferHandle = NULL;

// void Task1(void *pvParam)
// {
//     int i = 0;
//     int str_len;
//     int send_bytes = 0;

//     char tx_buff[50];

//     i++;
//     while (1)
//     {
//         i++;
//         str_len = sprintf(tx_buff, "Hello , I am Chenzy's NUM %d", i); //初始字符串函数，并返回字符串长度
//         send_bytes = xStreamBufferSend(StreamBufferHandle,
//                                        (void *)tx_buff,
//                                        str_len,
//                                        portMAX_DELAY);
//         printf("---------------------\n");
//         printf("Send str_len = %d ,send_bytes = %d\n", str_len, send_bytes);

//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
// }

// void Task2(void *pvParam)
// {
//     char rx_buff[50];
//     int rec_bytes = 0;
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(3000));
//         memset(rx_buff, 0, sizeof(rx_buff)); //初始化接收数据的buff，将值都置零

//         rec_bytes = xStreamBufferReceive(StreamBufferHandle,
//                              (void *)rx_buff,
//                              sizeof(rx_buff),
//                              portMAX_DELAY);
//         printf("---------------------\n");
//         printf("Reseive rec_bytes = %d ,data = %s\n", rec_bytes, rx_buff);
        
//     }
// }

// void app_main(void)
// {
//     StreamBufferHandle = xStreamBufferCreate(1000, 27); // BUFF的大小为1000字节，让接收函数接收到27字节才能解除阻塞（block）

//     if (StreamBufferHandle != NULL)
//     {
//         vTaskSuspendAll(); // Create Task前先暂停（suspend）任务管理器
//         xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
//         xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 1, NULL);
//         xTaskResumeAll(); // Task创建后打开任务管理器，确保任务按设置的优先级执行
//     }
//     else
//     {
//         printf("---------------------\n");
//         printf("StreamBUFF fail !\n");
//     }
// }

// // ------------------------------------END STream BUFF--------------------------------------

// // -----------------------------START MESSSAGE BUFF ONE--------------------------------------------

// MessageBufferHandle_t MessageBufferHandle = NULL;

// void Task1(void *pvParam)
// {
//     int i = 0;
//     int str_len;
//     int send_bytes = 0;

//     char tx_buff[50];

//     for (int i=0 ; i<3 ; i++)
//     {
//         str_len = sprintf(tx_buff, "Hello , I am Chenzy's NO.: %d", i); //初始字符串函数，并返回字符串长度
//         send_bytes = xMessageBufferSend(MessageBufferHandle,
//                                        (void *)tx_buff,
//                                        str_len,
//                                        portMAX_DELAY);
//         printf("---------------------\n");
//         printf("Send i = %d ,send_bytes = %d\n", i, send_bytes);

//         vTaskDelay(pdMS_TO_TICKS(3000));
//     }
//     vTaskDelete(NULL);
// }

// void Task2(void *pvParam)
// {
//     char rx_buff[200];
//     int rec_bytes = 0;
//     while (1)
//     {
//         vTaskDelay(pdMS_TO_TICKS(3000));
//         memset(rx_buff, 0, sizeof(rx_buff)); //初始化接收数据的buff，将值都置零

//         rec_bytes = xMessageBufferReceive(MessageBufferHandle,
//                              (void *)rx_buff,
//                              sizeof(rx_buff),
//                              portMAX_DELAY);
//         printf("---------------------\n");
//         printf("Reseive rec_bytes = %d ,data = %s\n", rec_bytes, rx_buff);
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

// void app_main(void)
// {
//     MessageBufferHandle = xMessageBufferCreate(1000); // BUFF的大小为1000字节
//     if (MessageBufferHandle != NULL)
//     {
//         xTaskCreate(Task1, "Task1", 1024 * 5, NULL, 1, NULL);
//         xTaskCreate(Task2, "Task2", 1024 * 5, NULL, 1, NULL);
//     }
//     else
//     {
//         printf("---------------------\n");
//         printf("StreamBUFF fail !\n");
//     }
// }

// // ------------------------------------END MESSSAGE BUFF ONE-------------------------------------