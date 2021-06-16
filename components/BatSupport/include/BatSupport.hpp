#ifndef __BAT_SUPPORT_H
#define __BAT_SUPPORT_H 


#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

class BatSupport
{
private:
    const unsigned int DEFAULT_VREF = 3300; //Use adc2_vref_to_gpio() to obtain a better estimate
    const unsigned int NO_OF_SAMPLES = 32;  //Multisampling

    esp_adc_cal_characteristics_t *adc_chars;

    const adc_channel_t channel = ADC_CHANNEL_0; //GPIO34 if ADC1, GPIO14 if ADC2
    const adc_bits_width_t width = ADC_WIDTH_BIT_12;

    const adc_atten_t atten = ADC_ATTEN_DB_0;
    const adc_unit_t unit = ADC_UNIT_1;

public:
    BatSupport();
    uint32_t getBatVoltage();
};

#endif
