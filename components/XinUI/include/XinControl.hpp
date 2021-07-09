/*
 * @Author: Mcfly
 * @Date: 2021-07-08 15:54:01
 * @LastEditTime: 2021-07-09 14:59:15
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinControl.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_CONTROL_H
#define __XIN_CONTROL_H

#include "XinUIConstants.hpp"
class XinControl
{
protected:
    using XinXY = XinUIConstants::XinXY;
    
    XinXY centerXY, sizeXY;
    bool visible;
    XinControl(XinXY centerXY, XinXY sizeXY) : centerXY(centerXY), sizeXY(sizeXY), visible(1){};

public:
    XinControl *bottom = NULL, *top = NULL, *right = NULL, *left = NULL;
    
    virtual short getTopY() { return centerXY.y + sizeXY.y / 2; }
    virtual short getBottomY() { return centerXY.y - sizeXY.y / 2; }
    virtual short getRightX() { return centerXY.x + sizeXY.x / 2; }
    virtual short getLeftX() { return centerXY.x - sizeXY.x / 2; }
    virtual short getWidth() { return sizeXY.x; }
    virtual short getHeight() { return sizeXY.y; }
    virtual void draw() = 0;
};

#endif