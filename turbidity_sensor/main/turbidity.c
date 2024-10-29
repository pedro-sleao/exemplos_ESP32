/* TDS Sensor Example

   Mark Benson 2018

   Licence: MIT (or whatever is used by the rest of this project)
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_log.h"

#define TDS_ANALOG_GPIO     ADC1_CHANNEL_0 //ADC1 is availalbe on pins 15, 34, 35 & 36

#define TDS_TEMPERATURE             18.0  //(float) Temperature of water (we should measure this with a sensor to get an accurate reading)
#define TDS_VREF                    1.098   //(float) Voltage reference for ADC. We should measure the actual value of each ESP32

static const char *TAG = "SENSOR INFO";

void expose_vref(){
    // Expose the ADC VREF to a GPIO so we can measure it rather than assume it is 1.1v
    ESP_ERROR_CHECK(adc_vref_to_gpio(ADC_UNIT_2, GPIO_NUM_25));
    ESP_LOGI(TAG, "VREF routed to pin 25\n");
}

void config_pins(){
    ESP_LOGI(TAG, "Configure pins required for turbidity sensor.");
    // Pin to read the TDS sensor analog output
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(TDS_ANALOG_GPIO, ADC_ATTEN_DB_12);
}

int read_turbidity_sensor(){
    int analogSample = adc1_get_raw(TDS_ANALOG_GPIO);
    ESP_LOGI(TAG, "Analog read = %d", analogSample);
    return analogSample;
}


void turbidity_task(void * pvParameters){
    ESP_LOGI(TAG, "Turbidity Measurement Control Task: Starting");
    while(1){
        ESP_LOGI(TAG, "Turbidity Measurement Control Task: Read Sensor");
        float sensorReading = read_turbidity_sensor();
        printf("Turbidity Reading = %f \n", sensorReading);
        ESP_LOGI(TAG, "Turbidity Measurement Control Task: Sleeping 2 seconds");
        vTaskDelay((2000 / portTICK_PERIOD_MS));
    }
}

void app_main()
{
    // Enable sensor then wait 10 seconds for reading to stabilise before reading
    // Read the sensor a few times and get an average
    // Convert reading to a Total Disolved Solids (TDS) value
    //expose_vref();
    config_pins();
    // Start TDS Task
    xTaskCreate(turbidity_task, "turbidity_task", 2048, NULL, 5, NULL);
}