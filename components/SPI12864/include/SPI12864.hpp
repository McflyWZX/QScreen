/*
 * @Author: Mcfly
 * @Date: 2021-03-26 20:54:51
 * @LastEditTime: 2021-03-28 02:12:31
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\SPI12864\include\SPI12864.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __SPI12864_H
#define __SPI12864_H

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
class SPI12864
{
private:
    //测试变量 
    uint64_t i = 810000, ti;

    /* data */
    //OLED模式设置
    //0:4线串行模式
    //1:并行8080模式
    const uint8_t OLED_MODE = 0;
    const uint8_t SIZE = 16;
    const uint8_t XLevelL = 0x00;
    const uint8_t XLevelH = 0x10;
    const uint8_t Max_Column = 128;
    const uint8_t Max_Row = 64;
    const uint8_t Brightness = 0xFF;
    const uint8_t X_WIDTH = 128;
    const uint8_t Y_WIDTH = 64;

    const uint8_t OLED_CMD = 0;  //写命令
    const uint8_t OLED_DATA = 1; //写数据

    gpio_num_t PIN_NUM_DC;
    gpio_num_t PIN_NUM_RST;
    gpio_num_t PIN_NUM_CS;
    gpio_num_t PIN_NUM_MOSI;
    gpio_num_t PIN_NUM_CLK;

    void OLED_CS_Clr()
    {
        gpio_set_level(PIN_NUM_CS, 0);
    } 
    void OLED_CS_Set()
    {
        gpio_set_level(PIN_NUM_CS, 1);
    }
    void OLED_RST_Clr()
    {
        gpio_set_level(PIN_NUM_RST, 0);
    }
    void OLED_RST_Set() 
    {
        gpio_set_level(PIN_NUM_RST, 1);
    }
    void OLED_DC_Clr() 
    {
        gpio_set_level(PIN_NUM_DC, 0);
    }
    void OLED_DC_Set() 
    {
        gpio_set_level(PIN_NUM_DC, 1);
    }
    void OLED_SDIN_Clr() 
    {
        gpio_set_level(PIN_NUM_MOSI, 0);
    }
    void OLED_SDIN_Set() 
    {
        gpio_set_level(PIN_NUM_MOSI, 1);
    }
    void OLED_SCLK_Clr() 
    {
        gpio_set_level(PIN_NUM_CLK, 0);
    }
    void OLED_SCLK_Set() 
    {
        gpio_set_level(PIN_NUM_CLK, 1);
    }
    uint32_t oled_pow(uint8_t m, uint8_t n);
    void writeByte(uint8_t dat, uint8_t cmd);
public:
    SPI12864(gpio_num_t Pin_DC, gpio_num_t Pin_RST, gpio_num_t Pin_CS, gpio_num_t Pin_MOSI, gpio_num_t Pin_CLK);
    ~SPI12864();
    void displayOn(void);
    void displayOff(void);
    void clear(void);
    //void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
    //void fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    void showChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size);
    void showNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
    void showString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t size);
    void setPos(unsigned char x, unsigned char y);
    void showCHinese(uint8_t x, uint8_t y, uint8_t no);
    void drawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
    void screenUpdate();
};

#endif
