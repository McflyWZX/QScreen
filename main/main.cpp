/*
 * @Author: Mcfly
 * @Date: 2021-03-26 19:11:07
 * @LastEditTime: 2021-07-12 11:47:13
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
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "freertos/event_groups.h"
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
#include "XinBar.hpp"
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

#define EXAMPLE_ESP_WIFI_SSID      {'M', 'c', 'f', 'l', 'y', '\0'}
#define EXAMPLE_ESP_WIFI_PASS      {'2', '0', '0', '0', '1', '0', '0', '5', '\0' }
#define EXAMPLE_ESP_MAXIMUM_RETRY  5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";

static int s_retry_num = 0;

esp_err_t event_handler(void *ctx, system_event_t *event)
{
    return ESP_OK;
}

void wifi_init_sta(void)
{
    //esp_nvs_init();
    tcpip_adapter_init();
    ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
    wifi_config_t sta_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .bssid_set = false
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_config(WIFI_IF_STA, &sta_config) );
    ESP_ERROR_CHECK( esp_wifi_start() );
    ESP_ERROR_CHECK( esp_wifi_connect() );

}

void timeUpdate(void *winBar)
{
    XinBar *bar = (XinBar*)winBar;
    while(1)
    {
        bar->updateTime(1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

extern "C"
{
    unsigned char screenBuf[128 * 128 * 2];
    void app_main(void)
    {
        nvs_flash_init();
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

        XinText textQ(XinXY(0, 0), XinXY(100, 16), "QScreen Sunny", WHITE);
        win.add(&textQ);

        XinBar winBar(bspCard);
        win.add(&winBar);
        winBar.update();
        
        XinCorePicture::Bmp24Raw *wifiBmpRaw = bspCard.loadBmp("/sdcard", "wifi.bmp");
        XinPIC wifiPic(XinXY(0, 0), XinCorePicture::bmp24Raw2Bmp2(*wifiBmpRaw));
        delete wifiBmpRaw;
        wifiPic.setBmp2Color(BLUE);
        winBar.add(&wifiPic);

        XinCorePicture::Bmp24Raw *testBmpRaw = bspCard.loadBmp("/sdcard", "sunny.bmp");
        XinPIC testPIC(XinCorePicture::bmp24Raw2Bmp16(*testBmpRaw));
        delete testBmpRaw;
        testPIC.setAlphaColor(WHITE);
        win.add(&testPIC);
        XinText textSunny(XinXY(0, 0), XinXY(37, 16), "Sunny", GRAYBLUE);
        win.add(&textSunny);

        XinText text(XinXY(0, 0), XinXY(90, 16), "Love @GH!666(XinXin)", WHITE);
        win.add(&text);

        //xTaskCreatePinnedToCore( &timeUpdate, "ScreenUpdateTask", 90000, &winBar, 2, NULL , tskNO_AFFINITY);
        wifi_init_sta();
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
