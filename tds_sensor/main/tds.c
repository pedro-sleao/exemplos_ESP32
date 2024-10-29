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

static const char *TDS = "TDS INFO";

void expose_vref(){
    // Expose the ADC VREF to a GPIO so we can measure it rather than assume it is 1.1v
    ESP_ERROR_CHECK(adc_vref_to_gpio(ADC_UNIT_2, GPIO_NUM_25));
    ESP_LOGI(TDS, "VREF routed to pin 25\n");
}

void config_pins(){
    ESP_LOGI(TDS, "Configure pins required for TDS sensor.");
    // Pin to read the TDS sensor analog output
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(TDS_ANALOG_GPIO, ADC_ATTEN_DB_12);
}

int read_tds_sensor(){
    int analogSample = adc1_get_raw(TDS_ANALOG_GPIO);
    ESP_LOGI(TDS, "Analog read = %d", analogSample);
    return analogSample;
}

float convert_to_ppm(float analogReading){
    ESP_LOGI(TDS, "Converting an analog value to a TDS PPM value.");
    //https://www.dfrobot.com/wiki/index.php/Gravity:_Analog_TDS_Sensor_/_Meter_For_Arduino_SKU:_SEN0244#More_Documents
    float adcCompensation = 1 + (1/3.9); // 1/3.9 (11dB) attenuation.
    float vPerDiv = (TDS_VREF / 4096) * adcCompensation; // Calculate the volts per division using the VREF taking account of the chosen attenuation value.
    float averageVoltage = analogReading * vPerDiv; // Convert the ADC reading into volts
    float compensationCoefficient=1.0+0.02*(TDS_TEMPERATURE-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
    float compensationVolatge = averageVoltage / compensationCoefficient;  //temperature compensation
    float tdsValue = (133.42 * compensationVolatge * compensationVolatge * compensationVolatge - 255.86 * compensationVolatge * compensationVolatge + 857.39 * compensationVolatge) * 0.5; //convert voltage value to tds value

    ESP_LOGI(TDS, "Volts per division = %f", vPerDiv);
    ESP_LOGI(TDS, "Average Voltage = %f", averageVoltage);
    ESP_LOGI(TDS, "Temperature (currently fixed, we should measure this) = %f", TDS_TEMPERATURE);
    ESP_LOGI(TDS, "Compensation Coefficient = %f", compensationCoefficient);
    ESP_LOGI(TDS, "Compensation Voltge = %f", compensationVolatge);
    ESP_LOGI(TDS, "tdsValue = %f ppm", tdsValue);
    return tdsValue;
}

void tds_task(void * pvParameters){
    ESP_LOGI(TDS, "TDS Measurement Control Task: Starting");
    while(1){
        ESP_LOGI(TDS, "TDS Measurement Control Task: Read Sensor");
        float sensorReading = read_tds_sensor();
        float tdsResult = convert_to_ppm(sensorReading);
        printf("TDS Reading = %f ppm\n", tdsResult);
        ESP_LOGI(TDS, "TDS Measurement Control Task: Sleeping 2 seconds");
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
    xTaskCreate(tds_task, "tds_task", 2048, NULL, 5, NULL);
}