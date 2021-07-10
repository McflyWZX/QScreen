/*
 * @Author: Mcfly
 * @Date: 2021-07-02 22:30:21
 * @LastEditTime: 2021-07-10 14:28:40
 * @LastEditors: Mcfly
 * @Description: 提供XinOS的图片处理内核
 * @FilePath: \QScreen\components\XinCore\include\XinCorePicture.hpp
 * 本人github:https://github.com/McflyWZX
 */

#ifndef __XIN_CORE_PICTURE_H
#define __XIN_CORE_PICTURE_H

#include "XinUIConstants.hpp"

class XinCorePicture
{
private:
    static unsigned short RGB24TORGB16(unsigned char R, unsigned char G, unsigned char B) { return (unsigned short)((((R) >> 3) << 11) | (((G) >> 2) << 5) | ((B) >> 3)); }

public:
    struct Bmp24Raw
    {
        int width;
        int height;
        int rowSize;
        unsigned char *bmpData;
        Bmp24Raw(int w, int h, int rSize, unsigned char *data) : width(w), height(h), rowSize(rSize), bmpData(data) {}
        ~Bmp24Raw() {delete bmpData;}
    };

    struct Bmp16
    {
        int width;
        int height;
        unsigned char *bmpData;
        Bmp16(int w, int h, unsigned char *data) : width(w), height(h), bmpData(data) {}
        ~Bmp16() {delete bmpData;}
        unsigned short getPixel(int x, int y) { return ((unsigned short *)bmpData)[x + y * width]; }
        void setPixel(int x, int y, unsigned short color) { ((unsigned short *)bmpData)[x + y * width] = color; }
    };

    struct Bmp2
    {
        int width;
        int height;
        unsigned char *bmpData;
        Bmp2(int w, int h, unsigned char *data) : width(w), height(h), bmpData(data) {}
        ~Bmp2() {delete bmpData;}
        unsigned short getPixel(int x, int y) { return ((unsigned short *)bmpData)[x + y * width]; }
        void setPixel(int x, int y, unsigned short color) { ((unsigned short *)bmpData)[x + y * width] = color; }
    };

    XinCorePicture();
    static XinCorePicture::Bmp16 *bmp24Raw2Bmp16(Bmp24Raw &bmpRaw);
    static XinCorePicture::Bmp2 *bmp24Raw2Bmp2(Bmp24Raw &bmpRaw);
    static XinCorePicture::Bmp16 *nearestNeighborZoom(Bmp16 &srcBmp, int desiredX, int desiredY);
    static XinCorePicture::Bmp16 **splitBmp16(Bmp16 &srcBmp, int numX, int numY);
    static XinCorePicture::Bmp2 **splitBmp2(Bmp2 &srcBmp, int numX, int numY);
};

#endif