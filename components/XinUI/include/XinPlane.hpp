/*
 * @Author: Mcfly
 * @Date: 2021-07-08 14:18:08
 * @LastEditTime: 2021-07-09 14:35:07
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinPlane.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_PLANE_H
#define __XIN_PLANE_H

class XinPlane
{
private:
    short width;
    short height;
    unsigned char bytesPrePixel;
    unsigned char *vram;
    unsigned char layer;

    int coor2Index(unsigned int x, unsigned int y) { return y * width * bytesPrePixel + x; }

protected:
    XinPlane(short w, short h, unsigned char bytesPrePixel) : width(w), height(h), bytesPrePixel(bytesPrePixel){};
};

#endif