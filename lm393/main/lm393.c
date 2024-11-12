#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#define SENSOR_ANALOG_GPIO     ADC1_CHANNEL_0 //ADC1 is availalbe on pins 15, 34, 35 & 36

static const char *TAG = "SENSOR";

void config_pins(){
    ESP_LOGI(TAG, "Configure pins required for solo moisture sensor.");
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(SENSOR_ANALOG_GPIO, ADC_ATTEN_DB_12);
}

int read_sensor(){
    int analogSample = adc1_get_raw(SENSOR_ANALOG_GPIO);
    ESP_LOGI(TAG, "Analog read = %d", analogSample);
    return analogSample;
}

void sensor_task(void * pvParameters){
    ESP_LOGI(TAG, "Solo Moisture Measurement Control Task: Starting");
    while(1){
        ESP_LOGI(TAG, "Solo Moisture Measurement Control Task: Read Sensor");
        int sensorReading = read_tds_sensor();
        printf("Solo Moisture Reading = %d \n", sensorReading);
        ESP_LOGI(TAG, "Solo Moisture Measurement Control Task: Sleeping 2 seconds");
        vTaskDelay((2000 / portTICK_PERIOD_MS));
    }
}

void app_main(void)
{
    config_pins();

    xTaskCreate(sensor_task, "sensor_task", 2048, NULL, 5, NULL);

}