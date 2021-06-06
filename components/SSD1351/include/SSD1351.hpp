#ifndef __SSD1351_H
#define __SSD1351_H 

#include "stdlib.h"	
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include <sstream>
class SSD1351
{
private:
    unsigned short backColor, pointColor;   //背景色，画笔色
    //-----------------测试LED端口定义---------------- 
    gpio_num_t PIN_NUM_DC       = ((gpio_num_t)5);
    gpio_num_t PIN_NUM_RST      = ((gpio_num_t)18);
    gpio_num_t PIN_NUM_MOSI     = ((gpio_num_t)23);
    gpio_num_t PIN_NUM_CLK      = ((gpio_num_t)19);

    //#define LED_ON GPIO_ResetBits(GPIOB,GPIO_Pin_8)
    //#define LED_OFF GPIO_SetBits(GPIOB,GPIO_Pin_8)

    //-----------------OLED端口定义---------------- 
    void OLED_RES_Clr()
    {
        gpio_set_level(PIN_NUM_RST, 0);
    }
    void OLED_RES_Set() 
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

    #define OLED_CMD  0	//写命令
    #define OLED_DATA 1	//写数据
    //#define unsigned char unsigned char
    //#define unsigned short unsigned int
    //#define unsigned int unsigned long
    #define LCD_W    128
    #define LCD_H    128

    unsigned int mypow(unsigned char m,unsigned char n);
    void LCD_Writ_Bus(unsigned char dat);
    void LCD_WR_DATA8(unsigned short dat);
    void LCD_WR_DATA16(unsigned short dat);
    void LCD_WR_REG(unsigned char dat);
    void LCD_Address_Set(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
    void init(void); 
public:
    SSD1351();
    void setColor(unsigned short backColor, unsigned short pointColor);
    void clear(unsigned short Color);
    void showChinese(unsigned short x,unsigned short y,unsigned char index,unsigned char size,unsigned short color);
    void drawPoint(unsigned short x,unsigned short y,unsigned short color);
    void drawPointBig(unsigned short x,unsigned short y,unsigned short color);
    void fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color);
    void drawLine(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,unsigned short color);
    void drawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned short color);
    void circle(unsigned short x0,unsigned short y0,unsigned char r,unsigned short color);
    void showChar(unsigned short x,unsigned short y,unsigned char num,unsigned short color);
    void showString(unsigned short x,unsigned short y,const char *p,unsigned short color);
    void showNum(unsigned short x,unsigned short y,float num,unsigned char len,unsigned short color);
    void showPicture(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2);

    //颜色
    #define WHITE         	 0xFFFF
    #define BLACK         	 0x0000	  
    #define BLUE           	 0x001F  
    #define BRED             0XF81F
    #define GRED 			 0XFFE0
    #define GBLUE			 0X07FF
    #define RED           	 0xF800
    #define MAGENTA       	 0xF81F
    #define GREEN         	 0x07E0
    #define CYAN          	 0x7FFF
    #define YELLOW        	 0xFFE0
    #define BROWN 			 0XBC40 //棕色
    #define BRRED 			 0XFC07 //棕红色
    #define GRAY  			 0X8430 //灰色
    //GUI颜色

    #define DARKBLUE      	 0X01CF	//深蓝色
    #define LIGHTBLUE      	 0X7D7C	//浅蓝色  
    #define GRAYBLUE       	 0X5458 //灰蓝色
    //以上三色为PANEL的颜色 
    
    #define LIGHTGREEN     	 0X841F //浅绿色
    #define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

    #define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
    #define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
};
#endif
