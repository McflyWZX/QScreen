/*
 * @Author: Mcfly
 * @Date: 2021-06-19 00:49:28
 * @LastEditTime: 2021-07-09 14:32:18
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinWindow.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_WINDOW_H
#define __XIN_WINDOW_H

#include "XinPlane.hpp"
#include "XinUIConstants.hpp"

class XinWindows : public XinPlane
{
private:
    unsigned char backgroundMode; 
    XinBar topBar;
public:
    enum BackgroundMode {prueColor, bmp};
    XinWindows(short winW, short winH) : XinPlane(winW, winH, XinUIConstants::BytesPrePixel);
};

#endif