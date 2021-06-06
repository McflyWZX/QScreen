#include "SSD1351.hpp"
#include "SSD1351Font.h"
//#include "delay.h"
#include "SSD1351Bmp.h" 

void SSD1351::setColor(unsigned short backColor, unsigned short pointColor)
{
	this->backColor = backColor;
	this->pointColor = pointColor;
}
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void SSD1351::LCD_Writ_Bus(unsigned char dat) 
{	
	unsigned char i;	
  //OLED_CS_Clr();
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK_Clr();
		if(dat&0x80)
		   OLED_SDIN_Set();
		else 
		   OLED_SDIN_Clr();
		OLED_SCLK_Set();
		dat<<=1;   
	}		
  //OLED_CS_Set();
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void SSD1351::LCD_WR_DATA8(unsigned short dat)
{
	OLED_DC_Set();//写数据
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void SSD1351::LCD_WR_DATA16(unsigned short dat)
{
	OLED_DC_Set();//写数据
	LCD_Writ_Bus(dat>>8);
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void SSD1351::LCD_WR_REG(unsigned char dat)
{
	OLED_DC_Clr();//写命令
	LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void SSD1351::LCD_Address_Set(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
	LCD_WR_REG(0x15);//列地址设置
	LCD_WR_DATA8(x1);
	LCD_WR_DATA8(x2);
	LCD_WR_REG(0x75);//行地址设置
	LCD_WR_DATA8(y1);
	LCD_WR_DATA8(y2);
}

SSD1351::SSD1351()
{
    init();
    //printf("%d %d %d %d %d", screenPin.PIN_NUM_DC, screenPin.PIN_NUM_RST, screenPin.PIN_NUM_CS, screenPin.PIN_NUM_CLK, screenPin.PIN_NUM_MOSI);
}

//OLED的初始化
void SSD1351::init(void)
{

	gpio_pad_select_gpio(PIN_NUM_CLK);
    gpio_pad_select_gpio(PIN_NUM_MOSI);
    gpio_pad_select_gpio(PIN_NUM_DC);
    gpio_pad_select_gpio(PIN_NUM_RST);
    //gpio_pad_select_gpio(PIN_NUM_CS);

    gpio_set_direction(PIN_NUM_CLK, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_MOSI, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(PIN_NUM_RST, GPIO_MODE_OUTPUT);
    //gpio_set_direction(PIN_NUM_CS, GPIO_MODE_OUTPUT);

	OLED_RES_Clr();
	vTaskDelay(200 / portTICK_PERIOD_MS);
	OLED_RES_Set(); 
	
	LCD_WR_REG(0xFD);
	LCD_WR_DATA8(0x12);
	LCD_WR_REG(0xFD);
	LCD_WR_DATA8(0xB1);
	LCD_WR_REG(0xAE);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0xF1);
	LCD_WR_REG(0xCA);
	LCD_WR_DATA8(0x7F);
	LCD_WR_REG(0xA2);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xA1);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xA0);
	LCD_WR_DATA8(0x74);
	LCD_WR_REG(0xB5);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xAB);
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xB4);
	LCD_WR_DATA8(0xA0);
	LCD_WR_DATA8(0xB5);
	LCD_WR_DATA8(0x55);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0xC8);
	LCD_WR_DATA8(0x80);
	LCD_WR_DATA8(0xC8);
	LCD_WR_REG(0xC7);
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x32);
	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x17);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xB6);
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xBE);
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0xA6);
	LCD_WR_REG(0xAF);
}


/******************************************************************************
      函数说明：LCD清屏函数
      入口数据：无
      返回值：  无
******************************************************************************/
void SSD1351::clear(unsigned short Color)
{
	unsigned short i,j;  	
	LCD_Address_Set(0,0,LCD_W-1,LCD_H-1);
	LCD_WR_REG(0x5c);
    for(i=0;i<LCD_W;i++)
	  {
	     for (j=0;j<LCD_H;j++)
	     	{
        	LCD_WR_DATA16(Color);
	      }

	  }
}


/******************************************************************************
      函数说明：LCD显示汉字
      入口数据：x,y   起始坐标
                index 汉字的序号
                size  字号
      返回值：  无
******************************************************************************/
void SSD1351::showChinese(unsigned short x,unsigned short y,unsigned char index,unsigned char size,unsigned short color)
{  
	unsigned char i,j,x1=x;
	
	unsigned char *temp,size1;
	LCD_WR_REG(0x5c);
	if(size==16){temp=Hzk16;}//选择字号
	if(size==32){temp=Hzk32;}
  LCD_Address_Set(x,y,x+size-1,y+size-1);//设置一个汉字的区域
  size1=size*size/8;//一个汉字所占的字节
	temp+=index*size1;//写入的起始位置
	for(j=0;j<size1;j++)
	{
		for(i=0;i<8;i++)
		{
		 	if(*temp&(1<<i))//从数据的低位开始读
			{
				drawPoint(x,y,color);//点亮
			}
			else
			{
				drawPoint(x,y,backColor);
			}
			x++;
			if((x-x1)==size)
			{
				y++;
				x=x1;
			}
		}
		temp++;
	 }
}


/******************************************************************************
      函数说明：LCD显示汉字
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void SSD1351::drawPoint(unsigned short x,unsigned short y,unsigned short color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_REG(0x5c);
	LCD_WR_DATA16(color);
} 


/******************************************************************************
      函数说明：LCD画一个大的点
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void SSD1351::drawPointBig(unsigned short x,unsigned short y,unsigned short color)
{
	fill(x-1,y-1,x+1,y+1,color);
} 


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
      返回值：  无
******************************************************************************/
void SSD1351::fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color)
{          
	unsigned short i,j; 
	LCD_Address_Set(xsta,ysta,xend,yend);      //设置光标位置 
	LCD_WR_REG(0x5c);
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)
		{
			LCD_WR_DATA16(color);//设置光标位置 		
		}			
	} 					  	    
}


/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void SSD1351::drawLine(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2,unsigned short color)
{
	unsigned short t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0)incx=1; //设置单步方向 
	else if (delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//水平线 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		drawPoint(uRow,uCol,color);//画点
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void SSD1351::drawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned short color)
{
	drawLine(x1,y1,x2,y1,color);
	drawLine(x1,y1,x1,y2,color);
	drawLine(x1,y2,x2,y2,color);
	drawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
      返回值：  无
******************************************************************************/
void SSD1351::circle(unsigned short x0,unsigned short y0,unsigned char r,unsigned short color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	while(a<=b)
	{
		drawPoint(x0-b,y0-a,color);             //3           
		drawPoint(x0+b,y0-a,color);             //0           
		drawPoint(x0-a,y0+b,color);             //1                
		drawPoint(x0-a,y0-b,color);             //2             
		drawPoint(x0+b,y0+a,color);             //4               
		drawPoint(x0+a,y0-b,color);             //5
		drawPoint(x0+a,y0+b,color);             //6 
		drawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}


/******************************************************************************
      函数说明：显示字符
      入口数据：x,y    起点坐标
                num    要显示的字符
      返回值：  无
******************************************************************************/
void SSD1351::showChar(unsigned short x,unsigned short y,unsigned char num,unsigned short color)
{
	unsigned char pos,t,temp;
	unsigned short x1=x;
	LCD_WR_REG(0x5c);
	if(x>LCD_W-16||y>LCD_H-16)return;	    //设置窗口		   
	num=num-' ';//得到偏移后的值
	LCD_Address_Set(x,y,x+8-1,y+16-1);      //设置光标位置 
		for(pos=0;pos<16;pos++)
		{
		    temp=asc2_1608[(unsigned short)num*16+pos];		 //调用1608字体
			 for(t=0;t<8;t++)
		    {
		        if(temp&0x01)drawPoint(x+t,y+pos,color);//画一个点
					  else drawPoint(x+t,y+pos,BLACK);
		        temp>>=1;
		    }
		}
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y    起点坐标
                *p     字符串起始地址
      返回值：  无
******************************************************************************/
void SSD1351::showString(unsigned short x,unsigned short y,const char *p,unsigned short color)
{         
    while(*p!='\0')
    {       
        if(x>LCD_W-16){x=0;y+=16;}
        if(y>LCD_H-16){y=x=0;clear(pointColor);}
        showChar(x,y,*p,color);
        x+=8;
        p++;
    }  
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
unsigned int SSD1351::mypow(unsigned char m,unsigned char n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}


/******************************************************************************
      函数说明：显示数字
      入口数据：x,y    起点坐标
                num    要显示的数字
                len    要显示的数字个数
      返回值：  无
******************************************************************************/
void SSD1351::showNum(unsigned short x,unsigned short y,float num,unsigned char len,unsigned short color)
{         	
	unsigned char t,temp;
	unsigned char enshow=0;
	unsigned short num1;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			showChar(x+8*(len-2),y,'.',color);
			t++;
			len+=1;
		}
	 	showChar(x+8*t,y,temp+48,color);
	}
}


/******************************************************************************
      函数说明：显示40x40图片
      入口数据：x,y    起点坐标
      返回值：  无
******************************************************************************/
void SSD1351::showPicture(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
	int i,j;
	LCD_Address_Set(x1,y1,x2,y2);
	LCD_WR_REG(0x5c);
	for(i=0;i<1600;i++)
	{
		LCD_WR_DATA8(image[i*2+1]);
		LCD_WR_DATA8(image[i*2]);
	}			
}
