/*
 * @Author: Mcfly
 * @Date: 2021-07-08 14:41:31
 * @LastEditTime: 2021-07-08 16:03:40
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinUIConstants.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_UI_CONSTANTS_H
#define __XIN_UI_CONSTANTS_H

namespace XinUIConstants{
    const unsigned char BytesPrePixel = 2;

    struct XinXY{
        int x;
        int y;
        XinXY(int x, int y) : x(x), y(y) {};
    };
}

#endif