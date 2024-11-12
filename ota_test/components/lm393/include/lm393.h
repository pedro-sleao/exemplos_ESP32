#ifndef LM393_H
#define LM393_H

#include <esp_err.h>
#include "driver/adc.h"

esp_err_t lm393_config(adc_bits_width_t width, adc1_channel_t channel, adc_atten_t atten);
esp_err_t lm393_read(int *value);

#endif
