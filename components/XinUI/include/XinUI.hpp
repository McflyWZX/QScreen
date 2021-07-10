/*
 * @Author: Mcfly
 * @Date: 2021-06-18 21:14:10
 * @LastEditTime: 2021-07-09 14:28:39
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinUI.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_UI_H
#define __XIN_UI_H 

class XinUI
{
private:
    const int  maxLayers = 10;
    const short widthPixels = 128;
    const short heightPixels = 128;
    const unsigned char bytesPrePixel = 2;
    unsigned char *vram;

public:
    XinUI();
    int addPlane(); //返回图层id

};

#endif
