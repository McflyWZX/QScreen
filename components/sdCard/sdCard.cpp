/*
 * @Author: Mcfly
 * @Date: 2021-06-16 19:42:35
 * @LastEditTime: 2021-07-01 23:40:05
 * @LastEditors: Mcfly
 * @Description: SD卡的板级支持及各种文件支持
 * @FilePath: \QScreen\components\sdCard\sdCard.cpp
 * 本人github:https://github.com/McflyWZX
 */
#include <stdio.h>
#include "sdCard.hpp"

using std::string;

void SdCardBsp::sdCardDet()
{
    esp_err_t ret = ESP_OK;
    ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &sdHost, &slot_config, &mount_config, &card);
    while (1)
    {
        do{
            vTaskDelay(400 / portTICK_PERIOD_MS);
        }
        while (SDbusy);
        SDbusy = 1; //SD卡检测需要占用SD卡资源，置高SDbusy
        ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &sdHost, &slot_config, &mount_config, &card);
        if (ret != ESP_OK)
        {
            if (ret == ESP_FAIL)
            {
                ESP_LOGE("SD_FS", "Failed to mount filesystem. ");
            }
            else
            {
                ESP_LOGE("SD_FS", "Failed :%s. ",
                         esp_err_to_name(ret));
            }
            hasCard = 0;
            gpio_set_level((gpio_num_t)17, 1);
        } else {
            hasCard = 1;
            gpio_set_level((gpio_num_t)17, 0);
        }
        esp_vfs_fat_sdmmc_unmount();
        SDbusy = 0;
    }
}

void detTaskHandle(void *mycard)
{
    ((SdCardBsp *)mycard)->sdCardDet();
}

void SdCardBsp::startDetCard()
{
    xTaskCreatePinnedToCore(&detTaskHandle, "sdCardDetTask", 90000, this, 2, NULL, tskNO_AFFINITY);
}

// void dir_file_list(string path, vector<string> &file_vec, string depth)
// {
//     DIR *dp;

//     struct dirent *dirp;
//     if ((dp = opendir(path.c_str())) == NULL)
//     {
//         perror("opendir error");
//         free(dp);
//         exit(1);
//     }

//     struct stat buf;
//     while ((dirp = readdir(dp)) != NULL)
//     {
//         if ((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))
//             continue;
//         string file_name = dirp->d_name;
//         if (file_name.find_last_of(".swp") == file_name.size() - 1)
//             continue;

//         file_name = path + "/" + file_name;
//         stat(file_name.c_str(), &buf);

//         if (S_ISDIR(buf.st_mode))
//         {
//             file_vec.push_back(depth + file_name + "/┐");
//             dir_file_list(string(file_name), file_vec, depth + "\t");
//         }
//         else
//         {

//             file_vec.push_back(depth + file_name);
//         }
//     }
//     closedir(dp);
// }

FILE *SdCardBsp::openFile(string fileSrc, string fileName, string opType)
{
    if (SDbusy || !hasCard)
    {
        printf("Card busy or no card\r\n");
        return NULL;
    }
    //尝试挂载文件系统
    SDbusy = 1;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount(mountPoint.c_str(), &sdHost, &slot_config, &mount_config, &card);
    if (ret != ESP_OK)
    {
        SDbusy = 0;
        hasCard = 0;
        printf("Mount failed\r\n");
        return NULL;
    }
    return fopen((fileSrc + "/" + fileName).c_str(), opType.c_str());
}

void SdCardBsp::closeFile(FILE *f)
{
    fclose(f);
    esp_vfs_fat_sdmmc_unmount();
    SDbusy = 0;
    return;
}

/**
 * @description: 打印BMP文件的头信息，用于调试
 * @param {*}
 * @return {*}
 * @Author: Mcfly
 * @Date: 2021-07-01 23:03:26
 */
void SdCardBsp::showBmpHead(BitmapFileHearder* pBmpHead)
{
    printf("位图文件头:\r\n");
    printf("文件大小:%d\r\n",(*pBmpHead).bfSize);
    printf("保留字:%d\r\n",(*pBmpHead).bfReserved1);
    printf("保留字:%d\r\n",(*pBmpHead).bfReserved2);
    printf("实际位图数据的偏移字节数:%d\r\n",(*pBmpHead).bfOffBits);    
}

/**
 * @description: 打印BMP文件的头信息，用于调试
 * @param {*}
 * @return {*}
 * @Author: Mcfly
 * @Date: 2021-07-01 23:02:13
 */
void SdCardBsp::showBmpInforHead(BitmapInfoHearder* pBmpInforHead)
{
    printf("位图信息头:\r\n");
    printf("结构体的长度:%d\r\n",(*pBmpInforHead).biSize);
    printf("位图宽:%d\r\n",(*pBmpInforHead).biWidth);
    printf("位图高:%d\r\n",(*pBmpInforHead).biHeight);
    printf("biPlanes平面数:%d\r\n",(*pBmpInforHead).biPlanes);
    printf("biBitCount采用颜色位数:%d\r\n",(*pBmpInforHead).biBitCount);
    printf("压缩方式:%d\r\n",(*pBmpInforHead).biCompression);
    printf("biSizeImage实际位图数据占用的字节数:%d\r\n",(*pBmpInforHead).biSizeImage);
    printf("X方向分辨率:%d\r\n",(*pBmpInforHead).biXPelsPerMeter);
    printf("Y方向分辨率:%d\r\n",(*pBmpInforHead).biYPelsPerMeter);
    printf("使用的颜色数:%d\r\n",(*pBmpInforHead).biClrUsed);
    printf("重要颜色数:%d\r\n",(*pBmpInforHead).biClrImportant);
}

/**
  * @description: 加载位图，目前支持BMP888
  * @param fileSrc:文件目录
  * @param fileName:文件名
  * @return {*}
  * @Author: Mcfly
  * @Date: 2021-06-19 17:46:52
  */
void SdCardBsp::loadBmp(string fileSrc, string fileName)
{
    int i, j, k, mode;
    int width, height, l_width, l_height;

    unsigned char red, green, blue;
    unsigned char pColorData[960];
    BitmapFileHearder bitHead;
    BitmapInfoHearder bitInfoHead;
    unsigned short fileType;

    FILE *bmpFile = openFile(fileSrc, fileName, "r");

    if (bmpFile != NULL)
    {
        printf("Open file success\r\n");

        //读取位图文件头信息
        fread(&fileType, sizeof(WORD), 1, bmpFile);

        if (fileType != 0x4d42)
        {
            printf("file is not .bmp file!\r\n");
            return;
        }
        else
        {
            printf("Ok this is .bmp file\r\n");
        }

        fread(&bitHead, sizeof(BitmapFileHearder), 1, bmpFile);

        showBmpHead(&bitHead); //显示头文件
        printf("\r\n");

        //读取位图信息头信息
        fread(&bitInfoHead, sizeof(BitmapInfoHearder), 1, bmpFile);
        showBmpInforHead(&bitInfoHead);
        printf("\r\n");
    }
    else
    {
        printf("file open fail!\r\n");
        return;
    }

    width = bitInfoHead.biWidth;
    height = bitInfoHead.biHeight;

    if (width > height)
    {
        l_width = widthColorBytes(width * bitInfoHead.biBitCount); //计算位图的实际宽度并确保它为32的倍数
        l_height = height;
        printf("%d\r\n", l_width);
        printf("%d\r\n", l_height);
        if (0)//l_width > 960 - 3 * y)
        {
            printf("\n宽度过大)\n");
            return;
        }
        if (0)//l_height > 240 - 3 * x)
        {
            printf("\n高度过大\n");
            return;
        }
        mode = 1;
    }
    else
    {
        l_width = widthColorBytes(width * bitInfoHead.biBitCount); //计算位图的实际宽度并确保它为32的倍数
        l_height = height;
        printf("%d\r\n", l_width);
        printf("%d\r\n", l_height);
        if (0)//l_width > 720 - 3 * x)
        {
            printf("\nSORRY, PIC IS TOO BIG (<=240)\n");
            return;
        }
        if (0)//l_height > 320 - 3 * y)
        {
            printf("\nSORRY, PIC IS TOO BIG (<=320)\n");
            return;
        }
        mode = 0;
    }

    if (bitInfoHead.biBitCount >= 24)
    {

        //for (i = y; i < height + 1; i++)
        //{

            for (j = 0; j < l_width; j++) //将一行数据全部读入
            {
                fread(pColorData + j, sizeof(unsigned char), 1, bmpFile);
            }

            /*for (j = x; j < width; j++) //一行有效信息
            {
                k = j * 3; //一行中第K个像素的起点（地址）
                red = pColorData[k + 2];
                green = pColorData[k + 1];
                blue = pColorData[k];
                if (mode == 1)
                    MY_LCD_WR_Data(i, j, RGB24TORGB16(red, green, blue)); //写入LCD-GRAM	显示出来
                else
                    MY_LCD_WR_Data(j, i, RGB24TORGB16(red, green, blue)); //写入LCD-GRAM	显示出来
            }*/
        //}
        //lcd扫描方向复原
    }
    else
    {
        printf("SORRY, THIS PIC IS NOT A 24BITS REAL COLOR");
        return;
    }

    closeFile(bmpFile);
}

SdCardBsp::SdCardBsp()
{
    mount_config = {
        .format_if_mount_failed = false, //格式化内存卡如果文件系统挂载失败
        .max_files = 8,
        .allocation_unit_size = 16 * 1024};

    //ESP_LOGI("SD", "Initializing SD card");

    // Use settings defined above to initialize SD card and mount FAT filesystem.
    //ESP_LOGI("SD", "Using SDMMC peripheral");
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdHost = host;

    // This initializes the slot without card detect (CD) and write protect (WP) signals.
    // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
    slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_cd = (gpio_num_t)22;

    // To use 1-line SD mode, uncomment the following line:
    // slot_config.width = 1;

    gpio_set_pull_mode((gpio_num_t)15, GPIO_PULLUP_ONLY); // CMD, needed in 4- and 1- line modes
    gpio_set_pull_mode((gpio_num_t)2, GPIO_PULLUP_ONLY);  // D0, needed in 4- and 1-line modes
    gpio_set_pull_mode((gpio_num_t)4, GPIO_PULLUP_ONLY);  // D1, needed in 4-line mode only
    gpio_set_pull_mode((gpio_num_t)12, GPIO_PULLUP_ONLY); // D2, needed in 4-line mode only
    gpio_set_pull_mode((gpio_num_t)13, GPIO_PULLUP_ONLY); // D3, needed in 4- and 1-line modes

    gpio_pad_select_gpio((gpio_num_t)17);
    gpio_set_direction((gpio_num_t)17, GPIO_MODE_OUTPUT);
}
