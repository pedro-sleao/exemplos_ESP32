#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_BUILTIN 2
#define LED_1 GPIO_NUM_32

void task_LED1(void *param);

void app_main(void)
{
    xTaskCreate(task_LED1, "LED1", 2048, NULL, 1, NULL);

    gpio_reset_pin(LED_BUILTIN);
    gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_BUILTIN, 0);
        vTaskDelay(100/portTICK_PERIOD_MS);
        gpio_set_level(LED_BUILTIN, 1);
        vTaskDelay(100/portTICK_PERIOD_MS);
    }
}

void task_LED1(void *param) {
    gpio_reset_pin(LED_1);
    gpio_set_direction(LED_1, GPIO_MODE_OUTPUT);

    while (1) {
        gpio_set_level(LED_1, 0);
        vTaskDelay(250/portTICK_PERIOD_MS);
        gpio_set_level(LED_1, 1);
        vTaskDelay(250/portTICK_PERIOD_MS);
    }   

    vTaskDelete(NULL);
}
