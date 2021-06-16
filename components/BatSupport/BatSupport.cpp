#include <stdio.h>
#include "BatSupport.hpp"

uint32_t BatSupport::getBatVoltage()
{
    uint32_t adc_reading = 0;
    //Multisampling
    for (int i = 0; i < NO_OF_SAMPLES; i++)
    {
        adc_reading += adc1_get_raw((adc1_channel_t)channel);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    adc_reading /= NO_OF_SAMPLES;
    return esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
}

BatSupport::BatSupport()
{
    adc1_config_width(width);
    adc1_config_channel_atten((adc1_channel_t)channel, atten);
    adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);
}

