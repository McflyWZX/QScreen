/*
 * @Author: Mcfly
 * @Date: 2021-03-26 20:54:51
 * @LastEditTime: 2021-03-28 02:11:21
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\SPI12864\SPI12864.cpp
 * 本人github:https://github.com/McflyWZX
 */
#include <stdio.h>
#include "SPI12864.hpp"
#include "stdlib.h"
#include "memory.h"
#include "oledfont.h"
#include <sstream>

void SPI12864::screenUpdate()
{
    std::stringstream ss;
    ti = i%100 / 10;
    uint64_t tempT = i;
    uint32_t ms = i % 100;
    tempT /= 100;
    uint32_t s = tempT % 60;
    tempT /= 60;
    uint32_t m = tempT % 60;
    tempT /= 60;
    uint32_t h = tempT % 24;
    tempT /= 24;
    uint32_t d = tempT;
    i++;
    ss << "    Meeting across\n mountains and seas.\n\n" << "times of running:\n";
    ss << "day:" << d << ", " << h << ":" << m << ":" << s << "." << ms;
    ss << "\n" << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti << ti;
    showString(0, 1, (uint8_t*)ss.str().c_str(), 6);
    printf("屏幕显示任务，运行在核心：%d 上。\n", xPortGetCoreID());
    //printf(ss.str().c_str());
    vTaskDelay(10 / portTICK_PERIOD_MS);
}

//PIN_NUM_DC; GPIO_NUM_21
//PIN_NUM_RST;  GPIO_NUM_18
//PIN_NUM_CS; GPIO_NUM_15
//PIN_NUM_MOSI; GPIO_NUM_23
//PIN_NUM_CLK;  GPIO_NUM_19
/**
 * @description: SPI12864类的构造函数
 * @param {gpio_num_t} Pin_DC
 * @param {gpio_num_t} Pin_RST
 * @param {gpio_num_t} Pin_CS
 * @param {gpio_num_t} Pin_MOSI
 * @param {gpio_num_t} Pin_CLK
 * @return {SPI12864}
 * @Author: Mcfly
 * @Date: 2021-03-26 21:43:38
 */
SPI12864::SPI12864(gpio_num_t Pin_DC, gpio_num_t Pin_RST, gpio_num_t Pin_CS, gpio_num_t Pin_MOSI, gpio_num_t Pin_CLK)
{
    PIN_NUM_DC = Pin_DC;
    PIN_NUM_RST = Pin_RST;
    PIN_NUM_CS = Pin_CS;
    PIN_NUM_MOSI = Pin_MOSI;
    PIN_NUM_CLK = Pin_CLK;

    //Initialize non-SPI GPIOs
    gpio_pad_select_gpio(PIN_NUM_DC);
    gpio_pad_select_gpio(PIN_NUM_RST);
    gpio_pad_select_gpio(PIN_NUM_CS);
    gpio_pad_select_gpio(PIN_NUM_CLK);
    gpio_pad_select_gpio(PIN_NUM_MOSI);

    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_MOSI, GPIO_MODE_OUTPUT);

    //printf("%d %d %d %d %d", screenPin.PIN_NUM_DC, screenPin.PIN_NUM_RST, screenPin.PIN_NUM_CS, screenPin.PIN_NUM_CLK, screenPin.PIN_NUM_MOSI);

    OLED_RST_Set();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    OLED_RST_Clr();
    vTaskDelay(200 / portTICK_PERIOD_MS);
    OLED_RST_Set();

    writeByte(0xAE, OLED_CMD); //--turn off oled panel
    writeByte(0x00, OLED_CMD); //---set low column address
    writeByte(0x10, OLED_CMD); //---set high column address
    writeByte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    writeByte(0x81, OLED_CMD); //--set contrast control register
    writeByte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
    writeByte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    writeByte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    writeByte(0xA6, OLED_CMD); //--set normal display
    writeByte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
    writeByte(0x3f, OLED_CMD); //--1/64 duty
    writeByte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    writeByte(0x00, OLED_CMD); //-not offset
    writeByte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
    writeByte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
    writeByte(0xD9, OLED_CMD); //--set pre-charge period
    writeByte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    writeByte(0xDA, OLED_CMD); //--set com pins hardware configuration
    writeByte(0x12, OLED_CMD);
    writeByte(0xDB, OLED_CMD); //--set vcomh
    writeByte(0x40, OLED_CMD); //Set VCOM Deselect Level
    writeByte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
    writeByte(0x02, OLED_CMD); //
    writeByte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
    writeByte(0x14, OLED_CMD); //--set(0x10) disable
    writeByte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
    writeByte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
    writeByte(0xAF, OLED_CMD); //--turn on oled panel

    writeByte(0xAF, OLED_CMD); /*display ON*/
    clear();
    setPos(0, 0);
}

void SPI12864::writeByte(uint8_t dat, uint8_t cmd)
{
    if (cmd)
        OLED_DC_Set();
    else
        OLED_DC_Clr();
    OLED_CS_Clr();
    for (uint8_t i = 0; i < 8; i++)
    {
        OLED_SCLK_Clr();
        if (dat & 0x80)
            OLED_SDIN_Set();
        else
            OLED_SDIN_Clr();
        OLED_SCLK_Set();
        dat <<= 1;
    }

    OLED_CS_Set();
    OLED_DC_Set();
}

void SPI12864::displayOn(void)
{
    writeByte(0X8D, OLED_CMD); //SET DCDC命令
    writeByte(0X14, OLED_CMD); //DCDC ON
    writeByte(0XAF, OLED_CMD); //DISPLAY ON
}
void SPI12864::displayOff(void)
{
    writeByte(0X8D, OLED_CMD); //SET DCDC命令
    writeByte(0X10, OLED_CMD); //DCDC OFF
    writeByte(0XAE, OLED_CMD); //DISPLAY OFF
}
void SPI12864::clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        writeByte(0xb0 + i, OLED_CMD); //设置页地址（0~7）
        writeByte(0x02, OLED_CMD);     //设置显示位置—列低地址
        writeByte(0x10, OLED_CMD);     //设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            writeByte(0, OLED_DATA);
    } //更新显示
}

//m^n函数
uint32_t SPI12864::oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while (n--)
        result *= m;
    return result;
}

void SPI12864::showChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //得到偏移后的值
    if (x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (size == 16)
    {

        setPos(x, y);
        for (i = 0; i < 8; i++)
            writeByte(F8X16[c * 16 + i], OLED_DATA);
        setPos(x, y + 1);
        for (i = 0; i < 8; i++)
            writeByte(F8X16[c * 16 + i + 8], OLED_DATA);
    }
    else
    {
        setPos(x, y + 1);
        for (i = 0; i < 6; i++)
            writeByte(F6x8[c][i], OLED_DATA);
    }
}
void SPI12864::showNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                showChar(x + (size / 2) * t, y, ' ', size);
                continue;
            }
            else
                enshow = 1;
        }
        showChar(x + (size / 2) * t, y, temp + '0', size);
    }
}
void SPI12864::showString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        if (chr[j] == '\n')
        {
            x = 0;
            if (size == 16)
                y += 2;
            else
                y += 1;
        }
        else
        {
            showChar(x, y, chr[j], size);
            if (size == 16)
                x += 8;
            else
                x += 6;
            if (x > 120)
            {
                x = 0;
                if (size == 16)
                    y += 2;
                else
                    y += 1;
            }
        }
        j++;
    }
}
void SPI12864::setPos(unsigned char x, unsigned char y)
{
    writeByte(0xb0 + y, OLED_CMD);
    writeByte((((x + 2) & 0xf0) >> 4) | 0x10, OLED_CMD);
    writeByte(((x + 2) & 0x0f), OLED_CMD);
}
void SPI12864::showCHinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    setPos(x, y);
    for (t = 0; t < 16; t++)
    {
        writeByte(Hzk[2 * no][t], OLED_DATA);
        adder += 1;
    }
    setPos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        writeByte(Hzk[2 * no + 1][t], OLED_DATA);
        adder += 1;
    }
}
void SPI12864::drawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++)
    {
        setPos(x0, y);
        for (x = x0; x < x1; x++)
        {
            writeByte(BMP[j++], OLED_DATA);
        }
    }
}

SPI12864::~SPI12864()
{
}
