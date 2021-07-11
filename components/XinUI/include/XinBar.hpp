/*
 * @Author: Mcfly
 * @Date: 2021-06-19 00:49:36
 * @LastEditTime: 2021-07-11 10:10:38
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinBar.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_BAR_H
#define __XIN_BAR_H

#include "XinControl.hpp"
#include "XinUIConstants.hpp"
#include "XinText.hpp"
#include "XinPIC.hpp"
#include "XinLayout.hpp"
#include "sdCard.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

class XinBar : public XinControl
{
private:
    using XinXY = XinUIConstants::XinXY;
    using ifstream = std::ifstream;
    using string = std::string;

    XinLayout barLayout;
    XinText timeText;
    long long HMS; //时分秒
    int year;
    int month;

public:
    XinBar(SdCardBsp &card) : XinControl(XinXY(64, 8), XinXY(128, 16)), barLayout(XinXY(64, 8), XinXY(128, 16)), timeText(XinXY(100, 16), BROWN)
    {
        std::ifstream *timeIFSP = card.openFileIFstream("/sdcard", "sysTime.txt", std::ios::in);
        *timeIFSP >> HMS >> year >> month;
        card.closeFileIFstream(timeIFSP);
        string timeS = std::to_string(HMS / 60 / 60) + ":" + std::to_string(HMS / 60 % 60) + ":" + std::to_string(HMS % 60) + " " + std::to_string(year);
        timeText.setTxt(timeS);
    }
    void updateTime(int plusT)
    {
        HMS += plusT;
        string timeS = std::to_string(HMS / 60 / 60) + ":" + std::to_string(HMS / 60 % 60) + ":" + std::to_string(HMS % 60) + " " + std::to_string(year);
        timeText.setTxt(timeS);
    }
    void update()
    {
        barLayout.add(&timeText);
    }
    void add(XinControl *ctrl){barLayout.add(ctrl);}
    int getWidth() { return barLayout.getWidth(); }
    int getHeight() { return barLayout.getHeight(); }
    void setStartX(int startX)
    {
        barLayout.setStartX(startX);
        centerXY.x = startX + 1 + sizeXY.x / 2;
    }
    void setStartY(int startY)
    {
        barLayout.setStartY(startY);
        centerXY.y = startY + 1 + sizeXY.y / 2;
    }
    void setCenterX(int centerX)
    {
        barLayout.setCenterX(centerX);
        centerXY.x = centerX;
    }
    void setCenterY(int centerY)
    {
        barLayout.setCenterY(centerY);
        centerXY.y = centerY;
    }
    void draw(unsigned char *vram, XinXY vramSize)
    {
        barLayout.draw(vram, vramSize);
    }
};

#endif