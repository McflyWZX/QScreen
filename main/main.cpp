/*
 * @Author: Mcfly
 * @Date: 2021-03-26 19:11:07
 * @LastEditTime: 2021-07-11 00:40:45
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
#include "XinWindow.hpp"
#include "XinText.hpp"
#include "XinPIC.hpp"
#include "XinUIConstants.hpp"
#include "XinCorePicture.hpp"
#include "XinCoreGraphic.hpp"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define __cplusplus

using std::string;
using std::vector;
using XinUIConstants::XinXY;

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
        spiScreen.clear(BLACK);

        XinCoreGraphic::initBmpFont(bspCard);
        XinWindow win(XinXY(64, 64), XinXY(128, 128), 1);
        win.setBackgroundColor(XinCorePicture::RGB24TORGB16(237, 157, 178));
        XinText text(XinXY(0, 0), XinXY(90, 16), "Love @GH!666(XinXin)", WHITE);
        XinText text2(XinXY(0, 0), XinXY(120, 16), "6:23:45", WHITE);
        win.add(&text2);
        win.add(&text);
        //spiScreen.showDemo();
        while((!bspCard.isHasCard()) || (!bspCard.cardFree()));
        XinCorePicture::Bmp24Raw *testBmpRaw = bspCard.loadBmp("/sdcard", "test.bmp");
        XinPIC testPIC(XinCorePicture::bmp24Raw2Bmp16(*testBmpRaw));     
        delete testBmpRaw;
        testPIC.setAlphaColor(WHITE);
        win.add(&testPIC);
        
        /*while((!bspCard.isHasCard()) || (!bspCard.cardFree()))
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        XinCorePicture::Bmp24Raw* testBmpRaw1 = bspCard.loadBmp("/sdcard", "test.bmp");
        XinCorePicture::Bmp24Raw* testBmpRaw2 = bspCard.loadBmp("/sdcard", "testb.bmp");
        if(testBmpRaw1 == NULL)printf("bmp1 Err\r\n");
        if(testBmpRaw2 == NULL)printf("bmp2 Err\r\n");
        XinCorePicture::Bmp16* testBmp1 = XinCorePicture::bmp24Raw2Bmp16(*testBmpRaw1);
        XinCorePicture::Bmp16* testBmp2 = XinCorePicture::bmp24Raw2Bmp16(*testBmpRaw2);
        testBmp1 = XinCorePicture::nearestNeighborZoom(*testBmp1, 64, 64);
        testBmp2 = XinCorePicture::nearestNeighborZoom(*testBmp2, 64, 64);*/

        //gpio_pad_select_gpio((gpio_num_t)0);
        //gpio_set_direction((gpio_num_t)0, GPIO_MODE_INPUT);
        unsigned char *vram1 = new unsigned char[128 * 128 * 2];
        while (1)
        {
            win.draw(vram1, XinXY(128, 128));
            spiScreen.fillBuf(0, 0, 127, 127, vram1);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
