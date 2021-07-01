/*
 * @Author: Mcfly
 * @Date: 2021-03-26 19:11:07
 * @LastEditTime: 2021-07-01 23:33:21
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

#include "sdkconfig.h"
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
#include "freertos/timers.h"
#include "driver/spi_master.h"
#include "stdlib.h"
#include "esp_err.h"
#include "esp_log.h"
#include "sdCard.hpp"
#include "BatSupport.hpp"
#include <algorithm>
#include <inttypes.h>
#include <math.h>
#include <memory.h>

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define __cplusplus

using std::string;
using std::vector;

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

extern "C"
{
    unsigned char screenBuf[128 * 128 * 2];
    void app_main(void)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
        listSystemInfo();

        //SD卡初始化
        SdCardBsp bspCard;
        bspCard.startDetCard();

        //电池信息管理
        BatSupport myBat;

        SSD1351 spiScreen;
        spiScreen.showDemo();

        float omega = 0, biasX, biasY;
        short r = 15, R = 32;
        unsigned short color = CYAN;
        while((!bspCard.isHasCard()) || (!bspCard.cardFree()));
        bspCard.loadBmp("/sdcard", "test.bmp");
        while (1)
        {
            //sprintf(strBuf, "%.3f %s", myBat.getBatVoltage() * 4 / 1000.0f, bspCard.isHasCard() ? "Carded" : "NoCard");
            //spiScreen.showString(0, 0, strBuf, MAGENTA);
            memset(screenBuf, 0x00, 128 * 128 * 2);
            biasX = 64 + R * cosf(omega);
            biasY = 64 + R * sinf(omega);
            for(short j = biasY + r; j >= biasY - r; j--)
            {
                for(short i = biasX - r; i <= biasX + r; i++)
                {
                    if((j - biasY) * (j - biasY) + (i - biasX) * (i - biasX) > r * r)
                        continue;
                    else {
                        *((unsigned short*)(screenBuf + j * 256 + i * 2)) = color;
                        //*((unsigned short*)(screenBuf + (j-2*R) * 256 + (i-2*R) * 2)) = ~color;
                    }
                }
            }
            omega += 3.1415926535 / 48;
            spiScreen.fillBuf(0, 0, 127, 127, screenBuf);
            //vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}
