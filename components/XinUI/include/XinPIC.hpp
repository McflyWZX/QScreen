/*
 * @Author: Mcfly
 * @Date: 2021-06-19 00:48:55
 * @LastEditTime: 2021-07-11 00:37:02
 * @LastEditors: Mcfly
 * @Description:
 * @FilePath: \QScreen\components\XinUI\include\XinPIC.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_PIC_H
#define __XIN_PIC_H 

#include "XinControl.hpp"
#include "XinCorePicture.hpp"
#include "XinCoreGraphic.hpp"

class XinPIC : public XinControl
{
private:
    using XinXY = XinUIConstants::XinXY;
    using Bmp16 = XinCorePicture::Bmp16;
    using Bmp2 = XinCorePicture::Bmp2;

    Bmp16 *bmp16;
    Bmp2 *bmp2;
    unsigned short color;

public:
    XinPIC(XinXY centerXY, Bmp2 *bmp2)
        : XinControl(centerXY, XinXY(bmp2->width, bmp2->height)), bmp16(NULL), bmp2(bmp2) {}
    XinPIC(XinXY centerXY, Bmp16 *bmp16)
        : XinControl(centerXY, XinXY(bmp16->width, bmp16->height)), bmp16(bmp16), bmp2(NULL) {}
    XinPIC(Bmp2 *bmp2)
        : XinControl(XinXY(0, 0), XinXY(bmp2->width, bmp2->height)), bmp16(NULL), bmp2(bmp2) {}
    XinPIC(Bmp16 *bmp16)
        : XinControl(XinXY(0, 0), XinXY(bmp16->width, bmp16->height)), bmp16(bmp16), bmp2(NULL) {}
    void setBmp16(Bmp16 *bmp16);
    void setBmp2(Bmp2 *bmp2);
    void setAlphaColor(unsigned short color){this->color = color;}
    void setBmp2Color(unsigned short color){this->color = color;}
    void draw(unsigned char *vram, XinXY vramSize);
};

#endif