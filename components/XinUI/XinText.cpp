/*
 * @Author: Mcfly
 * @Date: 2021-07-10 15:36:13
 * @LastEditTime: 2021-07-11 03:02:30
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\XinText.cpp
 * 本人github:https://github.com/McflyWZX
 */
#include "XinText.hpp"

void XinText::setHeight() 
{
    txtLen = txt.length();
    sizeXY.y = 16;
    int txtWidth = 0, thisTrunWidth = 0;
    for(int i = 1; i < txtLen; i++)
    {
        if((txt.c_str())[i] != ' ')
        {
            thisTrunWidth = XinCoreGraphic::bmpFontEng[(txt.c_str())[i] - ' ']->trueWidth;
        } else {
            thisTrunWidth = 16;
        }
        txtWidth += thisTrunWidth;
        if(txtWidth >= sizeXY.x)
        {
            sizeXY.y += 16;
            txtWidth = thisTrunWidth;
        }
    }
    /*sizeXY.y = txtLen * 16 / sizeXY.x + 1;
    if(txtLen * 16 % sizeXY.x == 0)
        sizeXY.y -= 1;
    sizeXY.y *= 16;*/
}


void XinText::setTxt(string s)
{
    txt = s;
    setHeight();
    //printf("txt      W:%d;H:%d;x:%d;y:%d\r\n", sizeXY.x, sizeXY.y, centerXY.x, centerXY.y);
}

void XinText::draw(unsigned char *vram, XinXY vramSize)
{
    XinXY drawXY(0, 0);
    int i = 0, txtWidth = 0;

    for(drawXY.y = 0; drawXY.y < sizeXY.y; drawXY.y += 16)
    {
        for(drawXY.x = 0; drawXY.x < sizeXY.x; drawXY.x += txtWidth)
        {
            txtWidth = XinCoreGraphic::bmpFontEng[(txt.c_str())[i] - ' ']->trueWidth;
            XinCoreGraphic::drawText(vram, vramSize, centerXY - (sizeXY / 2 - XinXY(txtWidth / 2, 8)) + drawXY, (txt.c_str())[i++], color);
            if((txt.c_str())[i] == 0x00)
                return;
        }
    }
}