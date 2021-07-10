/*
 * @Author: Mcfly
 * @Date: 2021-07-10 22:05:56
 * @LastEditTime: 2021-07-11 00:29:21
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\XinPIC.cpp
 * 本人github:https://github.com/McflyWZX
 */

#include "XinPIC.hpp"

void XinPIC::setBmp16(Bmp16 *bmp16)
{
    if(bmp2 != NULL)
    {
        delete bmp2;
        bmp2 = NULL;
    }
    this->bmp16 = bmp16;
    sizeXY.x = bmp16->width;
    sizeXY.y = bmp16->height;
}

void XinPIC::setBmp2(Bmp2 *bmp2)
{
    if(bmp16 != NULL)
    {
        delete bmp16;
        bmp16 = NULL;
    }
    this->bmp2 = bmp2;
    sizeXY.x = bmp2->width;
    sizeXY.y = bmp2->height;
}

void XinPIC::draw(unsigned char *vram, XinXY vramSize)
{
    if(bmp16 != NULL)
    {
        XinCoreGraphic::drawBmp16(vram, vramSize, centerXY, bmp16, color);
    } else {
        XinCoreGraphic::drawBmp2(vram, vramSize, centerXY, bmp2, color);
    }
}