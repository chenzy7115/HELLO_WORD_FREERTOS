/*
test API of freeRTOS.
Soc:ESP32
1.教训视频：
https://space.bilibili.com/1338335828/channel/collectiondetail?sid=79734
2.README包含freeRTOS主要函数的介绍
3.从start to end是一个测试模块，需要单独使用，h文件不修改
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "freertos/queue.h"
#include "freertos/timers.h"
#include "freertos/semphr.h"

// -----------------------------START计数器信号量（Count Semaphore)--------------------------------------------

SemaphoreHandle_t semaphrHandle;

void carInTask(void *pvParam)
{
    int emptySpace = 0;
    BaseType_t iResult;
    while (1)
    {
        emptySpace = uxSemaphoreGetCount(semaphrHandle);
        printf("Empty space = %d\n", emptySpace);
        iResult = xSemaphoreTake(semaphrHandle, 0); //获得资源，计数器-1

        if (iResult == pdPASS)
        {
            printf("One car in!\n");
        }
        else
        {
            printf("No space!\n");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void carOutTask(void *pvParam)
{
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(6000));
        xSemaphoreGive(semaphrHandle); //给出资源，计数器+1
        printf("One car out!\n");
        }
}

void app_main(void)
{
    semaphrHandle = xSemaphoreCreateCounting(5,5);//计数器最大值为5，初始计数值为5
    xSemaphoreGive(semaphrHandle); //给出资源，计数器+1。由于计数器已经时最大值，所以计数器值不变还是5
    xTaskCreate(carInTask, "carInTask", 1024 * 5, NULL, 1, NULL);
    xTaskCreate(carOutTask, "carOutTask", 1024 * 5, NULL, 1, NULL);
}

// ------------------------------------END计数器信号量（Count Semaphore)--------------------------------------

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