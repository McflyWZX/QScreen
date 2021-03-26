/*
 * @Author: Mcfly
 * @Date: 2021-03-26 20:54:51
 * @LastEditTime: 2021-03-26 23:29:36
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
#include "oled.hpp"

//extern ScreenPin screenPin;

class SPI12864
{
private:
    /* data */
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
    void showString(uint8_t x, uint8_t y, uint8_t *p, uint8_t size);
    void setPos(unsigned char x, unsigned char y);
    void showCHinese(uint8_t x, uint8_t y, uint8_t no);
    void drawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
};

#endif
