/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "esp_task_wdt.h"//增加watch dog控制函数

void myTask1(void *pvParam)
{
    while (1)
    {
        printf("task:111\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void myTask2(void *pvParam)
{
    esp_task_wdt_add(NULL);
    while (1)
    {
        printf("task:222\n");
        esp_task_wdt_reset();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // UBaseType_t iPriority = 0;
    TaskHandle_t pxTask1 = NULL;
    TaskHandle_t pxTask2 = NULL;

    xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask1); //创建一个task
    xTaskCreate(myTask2, "myTask2", 2048, NULL, 1, &pxTask2); //创建一个task

    UBaseType_t uxStack1;

    // while (1)
    // {
    //     uxStack1 = uxTaskGetStackHighWaterMark(pxTask1);
    //     printf("uxStack1:%d\n", uxStack1);
    //     vTaskDelay(3000 / portTICK_PERIOD_MS);
    // }
}

// vTaskSuspend(pxTask1);
// vTaskDelay(5000 / portTICK_PERIOD_MS);
// vTaskResume(pxTask1);

// vTaskDelay(3000 / portTICK_PERIOD_MS);
// vTaskSuspend(pxTask1);

// vTaskPrioritySet(pxTask, 3);

// iPriority = uxTaskPriorityGet(pxTask); //通过句柄获得task的优先级

// printf("Task1's value : iPriority : %d\n", iPriority);
