/*
 * @Author: Mcfly
 * @Date: 2021-03-26 20:54:51
 * @LastEditTime: 2021-03-26 23:40:14
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\SPI12864\SPI12864.cpp
 * 本人github:https://github.com/McflyWZX
 */
#include <stdio.h>
#include "SPI12864.hpp"

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
    screenPin.PIN_NUM_DC = Pin_DC;
    screenPin.PIN_NUM_RST = Pin_RST;
    screenPin.PIN_NUM_CS = Pin_CS;
    screenPin.PIN_NUM_MOSI = Pin_MOSI;
    screenPin.PIN_NUM_CLK = Pin_CLK;

    //Initialize non-SPI GPIOs
    gpio_pad_select_gpio(screenPin.PIN_NUM_DC);
    gpio_pad_select_gpio(screenPin.PIN_NUM_RST);
    gpio_pad_select_gpio(screenPin.PIN_NUM_CS);
    gpio_pad_select_gpio(screenPin.PIN_NUM_CLK);
    gpio_pad_select_gpio(screenPin.PIN_NUM_MOSI);

    gpio_set_direction(screenPin.PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(screenPin.PIN_NUM_RST, GPIO_MODE_OUTPUT);
    gpio_set_direction(screenPin.PIN_NUM_CS, GPIO_MODE_OUTPUT);
    gpio_set_direction(screenPin.PIN_NUM_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(screenPin.PIN_NUM_MOSI, GPIO_MODE_OUTPUT);

    printf("%d %d %d %d %d", screenPin.PIN_NUM_DC, screenPin.PIN_NUM_RST, screenPin.PIN_NUM_CS, screenPin.PIN_NUM_CLK, screenPin.PIN_NUM_MOSI);

    OLED_Init();
}

void SPI12864::displayOn(void)
{
    OLED_Display_On();
}
void SPI12864::displayOff(void)
{
    OLED_Display_Off();
}
void SPI12864::clear(void)
{
    OLED_Clear();
}
void SPI12864::showChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size)
{
    OLED_ShowChar(x, y, chr, size);
}
void SPI12864::showNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    OLED_ShowNum(x, y, num, len, size);
}
void SPI12864::showString(uint8_t x, uint8_t y, uint8_t *p, uint8_t size)
{
    OLED_ShowString(x, y, p, size);
}
void SPI12864::setPos(unsigned char x, unsigned char y)
{
    OLED_Set_Pos(x, y);
}
void SPI12864::showCHinese(uint8_t x, uint8_t y, uint8_t no)
{
    OLED_ShowCHinese(x, y, no);
}
void SPI12864::drawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    OLED_DrawBMP(x0, y0, x1, y1, BMP);
}

SPI12864::~SPI12864()
{
}
