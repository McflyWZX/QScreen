/*
 * @Author: Mcfly
 * @Date: 2021-06-19 00:49:28
 * @LastEditTime: 2021-07-10 19:55:10
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinWindow.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_WINDOW_H
#define __XIN_WINDOW_H

#include "XinPlane.hpp"
#include "XinLayout.hpp"
#include "XinUIConstants.hpp"
#include "XinCoreGraphic.hpp"

class XinWindow : public XinPlane
{
public:
    enum BackgroundMode
    {
        prueColor,
        bmp
    };

private:
    using XinXY = XinUIConstants::XinXY;

    BackgroundMode backgroundMode = BackgroundMode::prueColor;
    bool fullScreen;
    XinLayout winLayout;
    unsigned short backGroundColor = 0x0000;

public:
    XinWindow(XinXY centerXY, XinXY sizeXY, bool fullScreen)
        : XinPlane(centerXY, sizeXY, XinUIConstants::BytesPrePixel), fullScreen(fullScreen), winLayout(centerXY, sizeXY, XinLayout::LayoutMode::Vertical, XinLayout::AlignmentMode::Left){};
    void draw(unsigned char *vram, XinXY vramSize);
    void add(XinControl *ctrl) { winLayout.add(ctrl); }
    void remove(XinControl *ctrl) { winLayout.remove(ctrl); }
    void setBackgroundColor(unsigned short color) {backGroundColor = color;}
};

#endif