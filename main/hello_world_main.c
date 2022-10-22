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

// -----------------------------START队列集合（QUEUE SET)--------------------------------------------
void sendTasc1(void *pvParam)
{
    QueueHandle_t QHandle;
    QHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;

    int i = 111;
    while (1)
    {
        xStatus = xQueueSend(QHandle, &i, 0);
        if (xStatus != pdPASS)
            printf("Send1 fail!\n");
        else
            printf("Send1 done!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sendTasc2(void *pvParam)
{
    QueueHandle_t QHandle;
    QHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;

    int i = 222;
    while (1)
    {
        xStatus = xQueueSend(QHandle, &i, 0);
        if (xStatus != pdPASS)
            printf("Send2 fail!\n");
        else
            printf("Send2 done!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void recTasc(void *pvParam)
{
    QueueSetHandle_t QueueSet;
    QueueSet = (QueueSetHandle_t)pvParam;

    QueueSetMemberHandle_t  QueueDate;

    BaseType_t xStatus;

    int i = 0;
    while (1)
    {
        QueueDate = xQueueSelectFromSet(QueueSet,portMAX_DELAY);
        xStatus = xQueueReceive(QueueDate, &i, portMAX_DELAY); // portMAX_DELAY可以无限delay，未接收到数据时阻塞receive返回数据直到接收到数据时
        if (xStatus != pdPASS)
            printf("Receive fail!\n");
        else
            printf("Receive message:%d\n", i);
    }
}

void app_main(void)
{
    // TaskHandle_t pxTask1;
    QueueHandle_t QHandle1;
    QHandle1 = xQueueCreate(5, sizeof(int));

    QueueHandle_t QHandle2;
    QHandle2 = xQueueCreate(5, sizeof(int));

    QueueSetHandle_t QueueSet;
    QueueSet = xQueueCreateSet(10); // queue set的创建长度应该等于需要加入的quene长度的和

    xQueueAddToSet(QHandle1, QueueSet); //第一个队列加入SET
    xQueueAddToSet(QHandle2, QueueSet); //第二个队列加入SET

    if ((QHandle1 != NULL) && (QHandle2 != NULL) && (QueueSet != NULL)) //只有队列1、2和队列集合都create时
    {
        printf("Create queue successfully!\n");
        xTaskCreate(sendTasc1, "sendTasc", 1024 * 5, (void *)QHandle1, 1, NULL);
        xTaskCreate(sendTasc2, "sendTasc", 1024 * 5, (void *)QHandle2, 1, NULL);
        xTaskCreate(recTasc, "recTasc", 1024 * 5, (void *)QueueSet, 2, NULL); //获得QueueSet句柄
    }
    else
    {
        printf("Can't create queue!\n");
    }
}

// ------------------------------------END队列集合（QUEUE SET)---------------------------------------

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