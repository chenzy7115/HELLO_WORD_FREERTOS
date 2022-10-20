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

// -----------------------------------------------------------------------------------------------
void myTask(void *pvParam){
    while (1)
    {
        printf("Hello world! In the tack!\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}

void app_main(void){
    xTaskCreate(myTask,"myTask1",1024,NULL,1,NULL);
}