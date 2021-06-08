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

#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <string>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include "SSD1351.hpp"
#include "sdkconfig.h"
#include "freertos/timers.h"
#include "driver/spi_master.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "stdlib.h"
#include "sdmmc_cmd.h"
#include "sdkconfig.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define __cplusplus

using std::string;
using std::vector;

#define DEFAULT_VREF 3300 //Use adc2_vref_to_gpio() to obtain a better estimate
#define NO_OF_SAMPLES 64  //Multisampling

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

bool hasCard = 0;
bool SDbusy = 0;
void sdCardDet(void *null)
{
    string mountPoint = "/sdcard";
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    sdmmc_slot_config_t slot_config;
    //esp_err_t ret;
    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    mount_config = {
        .format_if_mount_failed = false, //格式化内存卡如果文件系统挂载失败
        .max_files = 8,
        .allocation_unit_size = 16 * 1024};
    sdmmc_card_t *card;
    ESP_LOGI("SD", "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
    // Please check its source code and implement error recovery when developing
    // production applications.
    ESP_LOGI("SD", "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_cd = (gpio_num_t)22;

    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;

    // GPIOs 15, 2, 4, 12, 13 should have external 10k pull-ups.
    // Internal pull-ups are not sufficient. However, enabling internal pull-ups
    // does make a difference some boards, so we do that here.
    gpio_set_pull_mode((gpio_num_t)15, GPIO_PULLUP_ONLY); // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode((gpio_num_t)2, GPIO_PULLUP_ONLY);  // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode((gpio_num_t)4, GPIO_PULLUP_ONLY);  // D1, needed in 4-line mode only
    gpio_set_pull_mode((gpio_num_t)12, GPIO_PULLUP_ONLY); // D2, needed in 4-line mode only
    gpio_set_pull_mode((gpio_num_t)13, GPIO_PULLUP_ONLY); // D3, needed in 4- and 1-line modes

    esp_err_t ret = ESP_OK;
    gpio_pad_select_gpio((gpio_num_t)17);
    gpio_set_direction((gpio_num_t)17, GPIO_MODE_OUTPUT);

    ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &host, &slot_config, &mount_config, &card);
    while (1)
    {
        if (ret != ESP_OK)
        {
            if (ret == ESP_FAIL)
            {
                ESP_LOGE("SD_FS", "Failed to mount filesystem. ");
            }
            else
            {
                ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &host, &slot_config, &mount_config, &card);
                ESP_LOGE("SD_FS", "Failed :%s. ",
                         esp_err_to_name(ret));
            }
            hasCard = 0;
            gpio_set_level((gpio_num_t)17, 1);
        } else {
            hasCard = 1;
            gpio_set_level((gpio_num_t)17, 0);
            if(!SDbusy)
            {
                esp_vfs_fat_sdmmc_unmount();
                ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &host, &slot_config, &mount_config, &card);
            }
        }
        vTaskDelay(400 / portTICK_PERIOD_MS);
    }
    //return;
}

extern "C"
{

    void app_main(void)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        listSystemInfo();
        //ADC初始化
        adc1_config_width(width);
        adc1_config_channel_atten((adc1_channel_t)channel, atten);
        adc_chars = (esp_adc_cal_characteristics_t *)calloc(1, sizeof(esp_adc_cal_characteristics_t));
        esp_adc_cal_characterize(unit, atten, width, DEFAULT_VREF, adc_chars);

        //SD卡初始化
        //initSD();
        xTaskCreate(&sdCardDet, "sdCardDetTask", 4096, (void *)1, 2, NULL);

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
            gpio_set_direction((gpio_num_t)0, GPIO_MODE_INPUT);
            gpio_pullup_en((gpio_num_t)0);
            while (1)
            {
                uint32_t adc_reading = 0;
                //Multisampling
                for (int i = 0; i < NO_OF_SAMPLES; i++)
                {
                    adc_reading += adc1_get_raw((adc1_channel_t)channel);
                    vTaskDelay(10 / portTICK_PERIOD_MS);
                }
                adc_reading /= NO_OF_SAMPLES;
                uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
                sprintf(strBuf, "%.3f %s", voltage * 4 / 1000.0f, hasCard ? "Carded" : "NoCard");
                spiScreen.showString(0, 0, strBuf, MAGENTA);
                //spiScreen.showString(0, 0, strBuf, MAGENTA);
            }
        }
    }
}
