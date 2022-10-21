/* 
test API of freeRTOS.
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "freertos/queue.h"

typedef struct A_STRUCT
{
    char id;
    char data;
} xStruct;

// ------------------------------------------------------------------------------
void sendTasc(void *pvParam)
{
    QueueHandle_t QHandle;
    QHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    
    char *pStrToSend;
    int i = 0;
    while (1)
    {
        pStrToSend = (char *)malloc(50);
        snprintf(pStrToSend,50,"Today is good day %d!\r\n",i);
        i++;

        xStatus = xQueueSend(QHandle, &pStrToSend, 0);
        if (xStatus != pdPASS)
            printf("Send fail!\n");
        else
            printf("Send done!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void recTasc(void *pvParam)
{
    QueueHandle_t QHandle;
    QHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    
    char *pStrToRec;
    while (1)
    {
        if (uxQueueMessagesWaiting(QHandle) != 0)
        {
            xStatus = xQueueReceive(QHandle, &pStrToRec, 0);
            if (xStatus != pdPASS)
                printf("Receive fail!\n");
            else
                printf("Receive message:%s",pStrToRec);
            free(pStrToRec);
        }
        else
        {
            printf("No Data!\n");
        }

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

// ------------------------------------------------------------------------------
void app_main(void)
{
    // TaskHandle_t pxTask1;
    QueueHandle_t QHandle;
    QHandle = xQueueCreate(5, sizeof(char *));

    if (QHandle != NULL)
    {
        printf("Create queue successfully!\n");
        xTaskCreate(sendTasc, "sendTasc", 1024 * 5, (void *)QHandle, 1, NULL);
        xTaskCreate(recTasc, "recTasc", 1024 * 5, (void *)QHandle, 1, NULL);
    }
    else
    {
        printf("Can't create queue!\n");
    }
}
