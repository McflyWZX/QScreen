/*
 * @Author: Mcfly
 * @Date: 2021-03-26 19:11:07
 * @LastEditTime: 2021-08-14 23:25:22
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
#include "esp_http_client.h"
#include "cJSON.h"
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

#define MAX_HTTP_OUTPUT_BUFFER 2048

string cityName = "UnConnect";
string weather = "UnConnect";
bool wifiBeep = 0;

static void http_test_task(void *pvParameters)
{

    //02-1 定义需要的变量
    char output_buffer[MAX_HTTP_OUTPUT_BUFFER] = {0}; //用于接收通过http协议返回的数据
    int content_length = 0;                           //http协议头的长度

    //02-2 配置http结构体

    //定义http配置结构体，并且进行清零
    esp_http_client_config_t config;
    memset(&config, 0, sizeof(config));

    //向配置结构体内部写入url
    static const char *URL = "https://api.seniverse.com/v3/weather/now.json?key=S3-DPZ4WqAV7CEZDv&location=jiangxiYiChun&language=en&unit=c";
    config.url = URL;

    //初始化结构体
    esp_http_client_handle_t client = esp_http_client_init(&config); //初始化http连接

    //设置发送请求
    esp_http_client_set_method(client, HTTP_METHOD_GET);

    //02-3 循环通讯
    const char* TAG = "HTTPt";
    
    while (1)
    {

        // 与目标主机创建连接，并且声明写入内容长度为0
        esp_err_t err = esp_http_client_open(client, 0);

        //如果连接失败
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to open HTTP connection: %s", esp_err_to_name(err));
        }
        //如果连接成功
        else
        {

            //读取目标主机的返回内容的协议头
            content_length = esp_http_client_fetch_headers(client);

            //如果协议头长度小于0，说明没有成功读取到
            if (content_length < 0)
            {
                ESP_LOGE(TAG, "HTTP client fetch headers failed");
            }

            //如果成功读取到了协议头
            else
            {
                printf("连接成功\n");
                //读取目标主机通过http的响应内容
                int data_read = esp_http_client_read_response(client, output_buffer, MAX_HTTP_OUTPUT_BUFFER);
                if (data_read >= 0)
                {
                    printf("响应读取成功\n");
                //     //打印响应内容，包括响应状态，响应体长度及其内容
                //     ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                //              esp_http_client_get_status_code(client),     //获取响应状态信息
                //              esp_http_client_get_content_length(client)); //获取响应信息长度
                    printf("data:%s\n", output_buffer);
                    //对接收到的数据作相应的处理
                    cJSON *root = NULL;
                    root = cJSON_Parse(output_buffer);

                    cJSON *cjson_item = cJSON_GetObjectItem(root, "results");
                    cJSON *cjson_results = cJSON_GetArrayItem(cjson_item, 0);
                    
                    cJSON *cjson_location = cJSON_GetObjectItem(cjson_results, "location");
                    cJSON *cjson_name = cJSON_GetObjectItem(cjson_location, "name");

                    cJSON *cjson_now = cJSON_GetObjectItem(cjson_results, "now");
                    cJSON *cjson_text = cJSON_GetObjectItem(cjson_now, "text");
                    cJSON *cjson_temperature = cJSON_GetObjectItem(cjson_now, "temperature");

                    cityName = cjson_name->valuestring;
                    weather = cjson_text->valuestring;
                    weather += " ";
                    weather += cjson_temperature->valuestring;
                    wifiBeep = !wifiBeep;

                    printf("%s\n", cjson_name->valuestring);
                    printf("%s 气温:%s°\n", cjson_text->valuestring, cjson_temperature->valuestring);
                    printf("%d\n", (int)cjson_temperature);
                    
                    cJSON_Delete(root);

                }
                //如果不成功
                else
                {
                    ESP_LOGE(TAG, "Failed to read response");
                }
            }
        }
        printf("Free heap: %d\n", esp_get_free_heap_size());
        //关闭连接
        esp_http_client_close(client);

        //延时，因为心知天气免费版本每分钟只能获取20次数据
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

#define EXAMPLE_ESP_WIFI_SSID                                       \
    {                                                               \
        'M', 'c', 'f', 'l', 'y', 'K', 'e', 'r', 'n', 'e', 'l', '\0' \
    }
#define EXAMPLE_ESP_WIFI_PASS                                       \
    {                                                               \
        '7', '4', '8', '5', '8', '5', '7', '4', 'w', 'z', 'x', '\0' \
    }
#define EXAMPLE_ESP_MAXIMUM_RETRY 5

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

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
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_config_t sta_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .bssid_set = false}};
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());
}

void timeUpdate(void *winBar)
{
    XinBar *bar = (XinBar *)winBar;
    while (1)
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
        //从SD卡加载字体
        XinCoreGraphic::initBmpFont(bspCard);
        XinWindow win(XinXY(64, 64), XinXY(128, 128), 1);
        win.setBackgroundColor(XinCorePicture::RGB24TORGB16(237, 157, 178));

        //TODO:完成XinBar
        XinBar winBar(bspCard);
        win.add(&winBar);
        winBar.update();
        //TODO:完成WIFI功能
        XinCorePicture::Bmp24Raw *wifiBmpRaw = bspCard.loadBmp("/sdcard", "wifi.bmp");
        XinPIC wifiPic(XinXY(0, 0), XinCorePicture::bmp24Raw2Bmp2(*wifiBmpRaw));
        delete wifiBmpRaw;
        wifiPic.setBmp2Color(BLUE);
        winBar.add(&wifiPic);
        //天气显示部分
        XinCorePicture::Bmp24Raw *testBmpRaw = bspCard.loadBmp("/sdcard", "sunny.bmp");
        XinPIC testPIC(XinCorePicture::bmp24Raw2Bmp16(*testBmpRaw));
        delete testBmpRaw;
        testPIC.setAlphaColor(WHITE);
        win.add(&testPIC);
        XinText textQ(XinXY(0, 0), XinXY(80, 16), cityName.c_str(), WHITE);
        win.add(&textQ);
        XinText textSunny(XinXY(0, 0), XinXY(80, 16), weather.c_str(), GRAYBLUE);
        win.add(&textSunny);

        XinText text(XinXY(0, 0), XinXY(90, 16), "Love @GH!666(XinXin)", WHITE);
        win.add(&text);

        //xTaskCreatePinnedToCore( &timeUpdate, "ScreenUpdateTask", 90000, &winBar, 2, NULL , tskNO_AFFINITY);
        wifi_init_sta();
        vTaskDelay(pdMS_TO_TICKS(3000));
        xTaskCreate(&http_test_task, "http_test_task", 8192, NULL, 5, NULL);
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
            textQ.setTxt(cityName.c_str());
            textSunny.setTxt(weather.c_str());
            wifiPic.setBmp2Color(wifiBeep ? BLUE : GREEN);
            win.draw(vram1, XinXY(128, 128));
            spiScreen.fillBuf(0, 0, 127, 127, vram1);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}
