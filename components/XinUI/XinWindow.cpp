/*
 * @Author: Mcfly
 * @Date: 2021-07-09 19:40:28
 * @LastEditTime: 2021-07-10 16:38:40
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\XinWindow.cpp
 * 本人github:https://github.com/McflyWZX
 */
#include "XinWindow.hpp"

void XinWindow::draw(unsigned char *vram, XinXY vramSize)
{
    switch (backgroundMode)
    {
    case BackgroundMode::prueColor:
        XinCoreGraphic::fillVramRGB16(vram, vramSize, backGroundColor);
        break;
    default:
        break;
    }
    winLayout.draw(vram, vramSize);
}