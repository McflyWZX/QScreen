/*
 * @Author: Mcfly
 * @Date: 2021-03-26 19:11:07
 * @LastEditTime: 2021-03-27 01:03:17
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
#include "SPI12864.hpp"
#include <sstream>
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO CONFIG_BLINK_GPIO

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

void screenUpdate(void *spiScreen)
{
    SPI12864 *screen = (SPI12864*)spiScreen;
    uint64_t i = 0, ti;
    while(1)
    {
        std::stringstream ss;
        ti = i%100 / 10;
        ss << "    Meeting across\n mountains and seas.\n\n" << "times of running:\n" << i++;
        ss << "\n" << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti;
        screen->showString(0, 1, (uint8_t*)ss.str().c_str(), 6);
        TaskHandle_t handle = xTaskGetCurrentTaskHandle();
        printf("屏幕显示任务，运行在核心：%d 上。\n", xTaskGetAffinity(handle ));
        //vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

extern "C"
{
    
void app_main(void)
{
    vTaskDelay(pdMS_TO_TICKS(100));
    listSystemInfo();
    SPI12864 *spiScreen = new SPI12864(GPIO_NUM_21, GPIO_NUM_18, GPIO_NUM_15, GPIO_NUM_23, GPIO_NUM_19);
    xTaskCreatePinnedToCore( &screenUpdate, "ScreenUpdateTask", 90000, spiScreen, 2, NULL , 1);
    while(1) {
        TaskHandle_t handle = xTaskGetCurrentTaskHandle();
        printf("主任务，运行在核心：%d 上。\n", xTaskGetAffinity(handle ));
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
}
