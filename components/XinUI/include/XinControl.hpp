/*
 * @Author: Mcfly
 * @Date: 2021-07-08 15:54:01
 * @LastEditTime: 2021-07-11 02:59:27
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinControl.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_CONTROL_H
#define __XIN_CONTROL_H

#include "XinUIConstants.hpp"
#include <cstdio>
class XinControl
{
protected:
    using XinXY = XinUIConstants::XinXY;
    
    XinXY centerXY, sizeXY;
    bool visible;
    XinControl(XinXY centerXY, XinXY sizeXY) : centerXY(centerXY), sizeXY(sizeXY), visible(1){};

public:
    XinControl *bottom = NULL, *top = NULL, *right = NULL, *left = NULL;

    virtual void setStartX(int startX) { centerXY.x = startX + 1 + sizeXY.x / 2;}
    virtual void setStartY(int startY) { centerXY.y = startY + 1 + sizeXY.y / 2;}
    virtual void setCenterX(int centerX) { centerXY.x = centerX;}
    virtual void setCenterY(int centerY) { centerXY.y = centerY;}
    virtual int getTopY() { return centerXY.y - sizeXY.y / 2; }
    virtual int getBottomY() { return centerXY.y + sizeXY.y / 2; }
    virtual int getRightX() { return centerXY.x + sizeXY.x / 2; }
    virtual int getLeftX() { return centerXY.x - sizeXY.x / 2; }
    virtual int getCenterX() { return centerXY.x; }
    virtual int getCenterY() { return centerXY.y; }
    virtual int getWidth() { return sizeXY.x; }
    virtual int getHeight() { return sizeXY.y; }
    virtual void draw(unsigned char *vram, XinXY vramSize) = 0;
};

#endif