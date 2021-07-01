/*
 * @Author: Mcfly
 * @Date: 2021-06-16 19:42:35
 * @LastEditTime: 2021-07-01 23:31:33
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\sdCard\include\sdCard.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __SD_CARD_H
#define __SD_CARD_H

#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string"

using std::string;

class SdCardBsp
{
private:
    //位图文件头信息结构定义
    //其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）
    struct BitmapFileHearder
    {
        unsigned int bfSize;     //文件大小
        unsigned short bfReserved1; //保留字，不考虑
        unsigned short bfReserved2; //保留字，同上
        unsigned int bfOffBits;  //实际位图数据的偏移字节数，即前三个部分长度之和
    };
    struct BitmapInfoHearder
    {
        unsigned int biSize;         //指定此结构体的长度，为40
        int biWidth;            //位图宽
        int biHeight;           //位图高
        unsigned short biPlanes;      //平面数，为1
        unsigned short biBitCount;    //采用颜色位数，可以是1，2，4，8，16，24新的可以是32
        unsigned int biCompression;  //压缩方式，可以是0，1，2，其中0表示不压缩
        unsigned int biSizeImage;    //实际位图数据占用的字节数
        int biXPelsPerMeter;    //X方向分辨率
        int biYPelsPerMeter;    //Y方向分辨率
        unsigned int biClrUsed;      //使用的颜色数，如果为0，则表示默认值(2^颜色位数)
        unsigned int biClrImportant; //重要颜色数，如果为0，则表示所有颜色都是重要的
    };
    //像素的数据类型
    struct RGB_PIXEL
    {
        unsigned char rgbBlue;
        unsigned char rgbGreen;
        unsigned char rgbRed;
    };
    struct tagRGBQUAD
    {
        unsigned char rgbBlue;     //该颜色的蓝色分量
        unsigned char rgbGreen;    //该颜色的绿色分量
        unsigned char rgbRed;      //该颜色的红色分量
        unsigned char rgbReserved; //保留值
    };

    bool hasCard = 0;
    bool SDbusy = 0;
    string mountPoint = "/sdcard";
    esp_vfs_fat_sdmmc_mount_config_t mount_config;
    sdmmc_slot_config_t slot_config;
    sdmmc_card_t *card;
    sdmmc_host_t sdHost;

    unsigned short RGB24TORGB16(unsigned int R, unsigned int G, unsigned int B) { return (unsigned short int)((((R) >> 3) << 11) | (((G) >> 2) << 5) | ((B) >> 3)); }

    FILE *openFile(string fileSrc, string fileName, string opType);
    void closeFile(FILE *f);

    int widthColorBytes(int bits) { return ((bits) + 31) / 32 * 4; }
    void showBmpHead(BitmapFileHearder* pBmpHead);
    void showBmpInforHead(BitmapInfoHearder* pBmpInforHead);

public:
    SdCardBsp();
    void sdCardDet();
    void startDetCard();
    bool isHasCard() { return hasCard; }
    bool cardFree() { return !SDbusy; }
    void loadBmp(string fileSrc, string fileName);
};
#endif
