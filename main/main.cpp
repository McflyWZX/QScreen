/*
 * @Author: Mcfly
 * @Date: 2021-03-26 19:11:07
 * @LastEditTime: 2021-04-02 21:36:53
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\main\main.cpp
 * 本人github:https://github.com/McflyWZX
 */
/*
 *  ┌─────────────────────────────────────────────────────────────┐
 *  │┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐│
 *  ││Esc│!1 │@2 │#3 │$4 │%5 │^6 │&7 │*8 │(9 │)0 │_- │+= │|\ │`~ ││
 *  │├───┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴───┤│
 *  ││ Tab │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │{[ │}] │ BS  ││
 *  │├─────┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴┬──┴─────┤│
 *  ││ Ctrl │ A │ S │ D │ F │ G │ H │ J │ K │ L │: ;│" '│ Enter  ││
 *  │├──────┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴────┬───┤│
 *  ││ Shift  │ Z │ X │ C │ V │ B │ N │ M │< ,│> .│? /│Shift │Fn ││
 *  │└─────┬──┴┬──┴──┬┴───┴───┴───┴───┴───┴──┬┴───┴┬──┴┬─────┴───┘│
 *  │      │Fn │ Alt │         Space         │ Alt │Win│   HHKB   │
 *  │      └───┴─────┴───────────────────────┴─────┴───┘          │
 *  └─────────────────────────────────────────────────────────────┘
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
//#include "SPI12864.hpp"
#include "SSD1351.hpp"
#include "sdkconfig.h"
#include "freertos/timers.h"
#include "driver/spi_master.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "stdlib.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define __cplusplus

#define DEFAULT_VREF 3300 //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 64   //Multisampling

static esp_adc_cal_characteristics_t *adc_chars;

static const adc_channel_t channel = ADC_CHANNEL_0; //GPIO34 if ADC1, GPIO14 if ADC2
static const adc_bits_width_t width = ADC_WIDTH_BIT_12;

static const adc_atten_t atten = ADC_ATTEN_DB_0;
static const adc_unit_t unit = ADC_UNIT_1;

char strBuf[100];

//SPI12864 *spiScreen;

void listSystemInfo(void)
{
    printf("山东大学崇新学堂19开放性创新实践I项目!\n");

    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Free heap: %d\n", esp_get_free_heap_size());
}

float t = 0;
void screenTimer(TimerHandle_t pxTimer)
{
    //spiScreen->screenUpdate();
    //spiScreen.showNum(10, 10, t, 5, MAGENTA);
    t += 0.1;
}

extern "C"
{

    void app_main(void)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        listSystemInfo();
        /*spiScreen = new SPI12864(GPIO_NUM_21, GPIO_NUM_22, GPIO_NUM_15, GPIO_NUM_23, GPIO_NUM_18);
    TimerHandle_t xTimer = xTimerCreate(    "ScreenTimer",       // Just a text name, not used by the kernel.
                                10 / portTICK_PERIOD_MS,   // The timer period in ticks.
                                pdTRUE,        // The timers will auto-reload themselves when they expire.
                                NULL,  // Assign each timer a unique id equal to its array index.
                                screenTimer // Each timer calls the same callback when it expires.
                            );
    // The scheduler has not started yet so a block time is not used.
    xTimerStart( xTimer, 0 );
    while(1) {
        printf("主任务，运行在核心：%d 上。\n", xPortGetCoreID());
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }*/
        adc1_config_width(width);
        adc1_config_channel_atten((adc1_channel_t)channel, atten);
        adc_chars = (esp_adc_cal_characteristics_t*)calloc(1, sizeof(esp_adc_cal_characteristics_t));
        esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

        SSD1351 spiScreen;
        unsigned short i, m;
        spiScreen.clear(WHITE);
        spiScreen.setColor(BLACK, WHITE);
        while (1)
        {
            spiScreen.clear(CYAN);
            spiScreen.showChinese(16, 0, 0, 32, RED); //中
            spiScreen.showChinese(48, 0, 1, 32, RED); //景
            spiScreen.showChinese(80, 0, 2, 32, RED); //园

            spiScreen.showChinese(8, 40, 0, 16, RED);   //中
            spiScreen.showChinese(24, 40, 1, 16, RED);  //景
            spiScreen.showChinese(40, 40, 2, 16, RED);  //园
            spiScreen.showChinese(56, 40, 3, 16, RED);  //电
            spiScreen.showChinese(72, 40, 4, 16, RED);  //子
            spiScreen.showChinese(88, 40, 5, 16, RED);  //科
            spiScreen.showChinese(104, 40, 6, 16, RED); //技

            spiScreen.showString(32, 60, "1.5 OLED", RED);
            spiScreen.showString(22, 80, "OLED_W:", RED);
            spiScreen.showNum(82, 80, 128, 6, RED);
            spiScreen.showString(22, 100, "OLED_H:", RED);
            spiScreen.showNum(82, 100, 128, 6, RED);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            spiScreen.clear(CYAN);
            for (m = 0; m < 3; m++)
            {
                for (i = 0; i < 3; i++)
                {
                    spiScreen.showPicture(4 + m * 40, 4 + i * 40, 4 + 39 + m * 40, 4 + 39 + i * 40);
                }
            }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            spiScreen.clear(BLACK);
            /*TimerHandle_t xTimer = xTimerCreate("ScreenTimer",            // Just a text name, not used by the kernel.
                                                100 / portTICK_PERIOD_MS, // The timer period in ticks.
                                                pdTRUE,                   // The timers will auto-reload themselves when they expire.
                                                NULL,                     // Assign each timer a unique id equal to its array index.
                                                screenTimer               // Each timer calls the same callback when it expires.
            );*/
            // The scheduler has not started yet so a block time is not used.
            //xTimerStart(xTimer, 0);
            while (1)
            {
                uint32_t adc_reading = 0;
                //Multisampling
                for (int i = 0; i < NO_OF_SAMPLES; i++)
                {
                    adc_reading += adc1_get_raw((adc1_channel_t)channel);
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                }
                adc_reading /= NO_OF_SAMPLES;
                //Convert adc_reading to voltage in mV
                uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
                sprintf(strBuf, "Voltage: %.3fV", voltage * 4 / 1000.0f);
                spiScreen.showString(0, 20, strBuf, MAGENTA);
                //printf("Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
                //vTaskDelay(pdMS_TO_TICKS(1000));
                vTaskDelay(4000 / portTICK_PERIOD_MS);
            }
        }
    }
}
