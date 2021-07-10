/*
 * @Author: Mcfly
 * @Date: 2021-07-09 22:02:03
 * @LastEditTime: 2021-07-11 00:31:14
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinCore\XinCoreGraphic.cpp
 * 本人github:https://github.com/McflyWZX
 */

#include "XinCoreGraphic.hpp"
#include "memory.h"

XinCoreGraphic::Bmp2** XinCoreGraphic::bmpFontEng = NULL;

void XinCoreGraphic::initBmpFont(SdCardBsp &card)
{
    while((!card.isHasCard()) || (!card.cardFree()))
    {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    Bmp24Raw* testBmpRawFont = card.loadBmp("/sdcard", "engFont.bmp");
    Bmp2* bmpFont = XinCorePicture::bmp24Raw2Bmp2(*testBmpRawFont);
    delete testBmpRawFont;
    XinCoreGraphic::bmpFontEng = XinCorePicture::splitBmp2(*bmpFont, 20, bmpFont->height / 16);
    delete bmpFont;
}

void XinCoreGraphic::fillVramRGB16(unsigned char *vram, XinXY sizeXY, unsigned short color)
{
    for(unsigned short *p = (unsigned short *)vram; p < sizeXY.x * sizeXY.y + (unsigned short *)vram; p++)
        *p = color;
}

void XinCoreGraphic::fillVramBmp16(unsigned char *vram, XinXY sizeXY, Bmp16 *bmpSrc)
{
    if(sizeXY.x != bmpSrc->width || sizeXY.y != bmpSrc->height)
        bmpSrc = XinCorePicture::nearestNeighborZoom(*bmpSrc, sizeXY.x, sizeXY.y);
    memcpy(vram, bmpSrc->bmpData, sizeXY.x * sizeXY.y * 2);
}

/**
 * @description: 
 * @param {unsigned char} *vram
 * @param {XinXY} sizeXY是vram的大小
 * @param {XinXY} posXY
 * @param {unsigned short} color
 * @return {*}
 * @Author: Mcfly
 * @Date: 2021-07-10 15:59:23
 */
void XinCoreGraphic::drawPointRGB16(unsigned char *vram, XinXY sizeXY, XinXY posXY, unsigned short color)
{
    if(posXY.isInner(sizeXY))
    {
        ((unsigned short*)vram)[posXY.x + posXY.y * sizeXY.x] = color;
    }
}
/**
 * @description: 
 * @param {unsigned char} *vram
 * @param {XinXY} sizeXY是vram的大小
 * @param {XinXY} posXY
 * @param {unsigned char} c
 * @param {unsigned short} color
 * @return {*}
 * @Author: Mcfly
 * @Date: 2021-07-10 15:59:49
 */
void XinCoreGraphic::drawText(unsigned char *vram, XinXY sizeXY, XinXY posXY, unsigned char c, unsigned short color)
{
    XinXY drawXY(0, 0), txtSize(bmpFontEng[c - ' ']->trueWidth, 16);
    for(drawXY.y = 0; drawXY.y < 16; drawXY.y++)
    {
        //ed为最后一个像素的x坐标，需要遍历到
        for(drawXY.x = bmpFontEng[c - ' ']->st; drawXY.x <= bmpFontEng[c - ' ']->ed; drawXY.x++)
        {
            if(XinCoreGraphic::bmpFontEng[c - ' ']->getPixel(drawXY.x, drawXY.y) == 1)
                drawPointRGB16(vram, sizeXY, posXY - (txtSize / 2) + drawXY, color);
        }
    }
}

void XinCoreGraphic::drawBmp2(unsigned char *vram, XinXY sizeXY, XinXY posXY, Bmp2 *bmp2, unsigned short color)
{
    XinXY drawXY(0, 0), bmpSize(bmp2->width, bmp2->height);
    for(drawXY.y = 0; drawXY.y < bmpSize.y; drawXY.y++)
    {
        for(drawXY.x = 0; drawXY.x < bmpSize.x; drawXY.x++)
        {
            if(bmp2->getPixel(drawXY.x, drawXY.y) == 1)
                drawPointRGB16(vram, sizeXY, posXY - (bmpSize / 2) + drawXY, color);
        }
    }
}

void XinCoreGraphic::drawBmp16(unsigned char *vram, XinXY sizeXY, XinXY posXY, Bmp16 *bmp16)
{
    XinXY drawXY(0, 0), bmpSize(bmp16->width, bmp16->height);
    for(drawXY.y = 0; drawXY.y < bmpSize.y; drawXY.y++)
    {
        for(drawXY.x = 0; drawXY.x < bmpSize.x; drawXY.x++)
        {
            drawPointRGB16(vram, sizeXY, posXY - (bmpSize / 2) + drawXY, bmp16->getPixel(drawXY.x, drawXY.y));
        }
    }
}

void XinCoreGraphic::drawBmp16(unsigned char *vram, XinXY sizeXY, XinXY posXY, Bmp16 *bmp16, unsigned short alphaColor)
{
    XinXY drawXY(0, 0), bmpSize(bmp16->width, bmp16->height);
    for(drawXY.y = 0; drawXY.y < bmpSize.y; drawXY.y++)
    {
        for(drawXY.x = 0; drawXY.x < bmpSize.x; drawXY.x++)
        {
            if(bmp16->getPixel(drawXY.x, drawXY.y) != alphaColor)
                drawPointRGB16(vram, sizeXY, posXY - (bmpSize / 2) + drawXY, bmp16->getPixel(drawXY.x, drawXY.y));
        }
    }
}
