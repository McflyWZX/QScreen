/*
 * @Author: Mcfly
 * @Date: 2021-03-26 20:56:19
 * @LastEditTime: 2021-03-26 23:29:01
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\SPI12864\include\oled.hpp
 * ����github:https://github.com/McflyWZX
 */
//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   :
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//              ˵��:
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              D0   ��PD6��SCL��
//              D1   ��PD7��SDA��
//              RES  ��PD4
//              DC   ��PD5
//              CS   ��PD3
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   :
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "stdlib.h"
//OLEDģʽ����
//0:4�ߴ���ģʽ
//1:����8080ģʽ
#define OLED_MODE 0
#define SIZE 16
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64
//-----------------ESP�˿ڶ���----------------
typedef struct {
    gpio_num_t PIN_NUM_DC;
    gpio_num_t PIN_NUM_RST;
    gpio_num_t PIN_NUM_CS;
    gpio_num_t PIN_NUM_MOSI;
    gpio_num_t PIN_NUM_CLK;
} ScreenPin;
extern ScreenPin screenPin;

#define OLED_HOST    VSPI_HOST

#define PARALLEL_LINES 16
//-----------------OLED�˿ڶ���----------------
#define OLED_CS_Clr() gpio_set_level(screenPin.PIN_NUM_CS, 0) //CS
#define OLED_CS_Set() gpio_set_level(screenPin.PIN_NUM_CS, 1)

#define OLED_RST_Clr() gpio_set_level(screenPin.PIN_NUM_RST, 0) //RES
#define OLED_RST_Set() gpio_set_level(screenPin.PIN_NUM_RST, 1)

#define OLED_DC_Clr() gpio_set_level(screenPin.PIN_NUM_DC, 0) //DC
#define OLED_DC_Set() gpio_set_level(screenPin.PIN_NUM_DC, 1)

#define OLED_SDIN_Clr() gpio_set_level(screenPin.PIN_NUM_MOSI, 0) //DC
#define OLED_SDIN_Set() gpio_set_level(screenPin.PIN_NUM_MOSI, 1)

#define OLED_SCLK_Clr() gpio_set_level(screenPin.PIN_NUM_CLK, 0) //DC
#define OLED_SCLK_Set() gpio_set_level(screenPin.PIN_NUM_CLK, 1)

#define OLED_CMD 0  //д����
#define OLED_DATA 1 //д����
//OLED�����ú���
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t size);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *p, uint8_t size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
#endif
