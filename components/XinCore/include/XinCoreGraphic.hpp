/*
 * @Author: Mcfly
 * @Date: 2021-07-09 21:24:55
 * @LastEditTime: 2021-07-11 00:30:54
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinCore\include\XinCoreGraphic.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_CORE_GRAPHIC_H
#define __XIN_CORE_GRAPHIC_H

#include "XinUIConstants.hpp"
#include "XinCorePicture.hpp"
#include "sdCard.hpp"

class XinCoreGraphic{
private:
    using XinXY = XinUIConstants::XinXY;
    using Bmp24Raw = XinCorePicture::Bmp24Raw;
    using Bmp16 = XinCorePicture::Bmp16;
    using Bmp2 = XinCorePicture::Bmp2;
    unsigned char BPP = XinUIConstants::BytesPrePixel;
    
public:
static Bmp2** bmpFontEng;
    static void initBmpFont(SdCardBsp &card);
    static void fillVramRGB16(unsigned char *vram, XinXY sizeXY, unsigned short color);
    static void fillVramBmp16(unsigned char *vram, XinXY sizeXY, Bmp16 *bmpSrc);
    static void drawPointRGB16(unsigned char *vram, XinXY sizeXY, XinXY posXY, unsigned short color);
    static void drawText(unsigned char *vram, XinXY sizeXY, XinXY posXY, unsigned char c, unsigned short color);
    static void drawBmp2(unsigned char *vram, XinXY sizeXY, XinXY posXY, Bmp2 *bmp2, unsigned short color);
    static void drawBmp16(unsigned char *vram, XinXY sizeXY, XinXY posXY, Bmp16 *bmp16);
    static void drawBmp16(unsigned char *vram, XinXY sizeXY, XinXY posXY, Bmp16 *bmp16, unsigned short alphaColor);
};

#endif