/*
 * @Author: Mcfly
 * @Date: 2021-07-08 14:18:08
 * @LastEditTime: 2021-07-10 16:49:00
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinPlane.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_PLANE_H
#define __XIN_PLANE_H

#include "XinUIConstants.hpp"
#include "XinControl.hpp"

class XinPlane : public XinControl
{
private:
    using XinXY = XinUIConstants::XinXY;

    unsigned char bytesPrePixel;
    unsigned char *vram;
    unsigned char layer;

    int coor2Index(XinXY coor) { return coor.y * sizeXY.x * bytesPrePixel + coor.x; }

protected:
    XinPlane(XinXY centerXY, XinXY sizeXY, unsigned char bytesPrePixel) 
    : XinControl(centerXY, sizeXY), bytesPrePixel(bytesPrePixel)
    {};

public:
    virtual void draw(unsigned char *vram, XinXY vramSize) = 0;
};

#endif