#include <stdio.h>
#include "tds.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

static const char *TAG = "tds";

static adc1_channel_t adc1_channel;

esp_err_t tds_config(adc_bits_width_t width, adc1_channel_t channel, adc_atten_t atten){
    ESP_LOGI(TAG, "Configure pins required for tds.");
    adc1_channel = channel;
    adc1_config_width(width);
    adc1_config_channel_atten(channel, atten);

    return ESP_OK;
}

esp_err_t tds_read(int *value){
    int analogSample = adc1_get_raw(adc1_channel);
    ESP_LOGI(TAG, "Analog read = %d", analogSample);

    *value = analogSample;
    return ESP_OK;
}
