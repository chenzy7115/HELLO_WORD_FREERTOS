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

static const char *pcTxt = "I am CHENZY.";

void myTask(void *pvParam)
{
    char *pcTxtInTask;
    pcTxtInTask = pvParam;

    printf("I got message: %s\n", pcTxtInTask);
        
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    vTaskDelete(NULL);
}

void app_main(void)
{

    xTaskCreate(myTask, "myTask1", 2048, (void *) pcTxt, 1, NULL);
}

