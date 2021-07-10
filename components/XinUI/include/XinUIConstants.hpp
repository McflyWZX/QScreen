/*
 * @Author: Mcfly
 * @Date: 2021-07-08 14:41:31
 * @LastEditTime: 2021-07-10 15:54:51
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinUIConstants.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_UI_CONSTANTS_H
#define __XIN_UI_CONSTANTS_H

#include "cstdio"

namespace XinUIConstants
{
    const unsigned char BytesPrePixel = 2;

    struct XinXY
    {
        int x;
        int y;
        XinXY(int x, int y) : x(x), y(y){};
        bool isInner(XinXY &sizeXY) { return sizeXY.x > x && x >= 0 && sizeXY.y > y && y >= 0; }
        XinXY operator-(const XinXY &otherXY) const { return XinXY(this->x - otherXY.x, this->y - otherXY.y); }
        XinXY operator+(const XinXY &otherXY) const { return XinXY(this->x + otherXY.x, this->y + otherXY.y); }
        XinXY operator/(const int number) const { return XinXY(this->x / number, this->y / number); }
        XinXY operator*(const int number) const { return XinXY(this->x * number, this->y * number); }
    };

    //int coor2Index(int x, int y, int widthPixels){return y * widthPixels * BytesPrePixel + x;}
}

#endif