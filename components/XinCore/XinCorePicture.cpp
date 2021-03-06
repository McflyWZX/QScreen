
/*
 * @Author: Mcfly
 * @Date: 2021-07-02 22:42:31
 * @LastEditTime: 2021-07-10 20:36:48
 * @LastEditors: Mcfly
 * @Description: 提供XinOS的图片处理内核
 * @FilePath: \QScreen\components\XinCore\XinCorePicture.cpp
 * 本人github:https://github.com/McflyWZX
 */

#include "XinCorePicture.hpp"
#include "cstdio"
#include "memory.h"

XinCorePicture::XinCorePicture()
{
}

/**
 * @description: 
 * @param {Bmp24Raw} &bmpRaw:24位原始位图数据
 * @return {Bmp16} 可直接使用的16位位图数据
 * @Author: Mcfly
 * @Date: 2021-07-02 23:00:24
 */
XinCorePicture::Bmp16 *XinCorePicture::bmp24Raw2Bmp16(Bmp24Raw &bmpRaw)
{
    //分配存储bmp16色彩数据的内存
    unsigned char *bmp16Data = new unsigned char[bmpRaw.width * bmpRaw.height * 2];

    unsigned char *rowStart, *rowEnd;
    unsigned short *bmp16Color = (unsigned short *)bmp16Data; //以16位的方式填写数据，效率更高
    //因为历史原因bmp图像y轴是倒着的，故倒过来读取，但是当height为负数时就是正着的，一般不会
    for (int y = bmpRaw.height - 1; y >= 0; y--)
    {
        rowStart = bmpRaw.bmpData + y * bmpRaw.rowSize;
        rowEnd = rowStart + bmpRaw.width * 3;
        for (unsigned char *xColor = rowStart; xColor < rowEnd; xColor += 3)
        {
            //迭代每一个RGB并把它写入16位系统位图数据中
            *(bmp16Color++) = RGB24TORGB16(*(xColor + 2), *(xColor + 1), *xColor);
        }
    }

    return new Bmp16(bmpRaw.width, bmpRaw.height, bmp16Data);
}
/**
 * @description: 
 * @param {Bmp24Raw} &bmpRaw
 * @return {*}
 * @Author: Mcfly
 * @Date: 2021-07-10 11:20:33
 */
XinCorePicture::Bmp2 *XinCorePicture::bmp24Raw2Bmp2(Bmp24Raw &bmpRaw)
{
    //分配存储bmp16色彩数据的内存
    unsigned char *bmp2Data = new unsigned char[bmpRaw.width * bmpRaw.height];

    unsigned char *rowStart, *rowEnd;
    unsigned char *bmp2Color = bmp2Data; //以16位的方式填写数据，效率更高
    unsigned int srcColor;
    //因为历史原因bmp图像y轴是倒着的，故倒过来读取，但是当height为负数时就是正着的，一般不会
    for (int y = bmpRaw.height - 1; y >= 0; y--)
    {
        rowStart = bmpRaw.bmpData + y * bmpRaw.rowSize;
        rowEnd = rowStart + bmpRaw.width * 3;
        for (unsigned char *xColor = rowStart; xColor < rowEnd; xColor += 3)
        {
            //迭代每一个RGB并把它写入16位系统位图数据中
            srcColor = 0;
            srcColor = (*xColor << 16) | (*(xColor + 1) << 8) | (*(xColor + 2));
            *(bmp2Color++) = (unsigned char)(srcColor == 0x000000 ? 0 : 1);
        }
    }

    return new Bmp2(bmpRaw.width, bmpRaw.height, bmp2Data);
}

/**
 * @description: 使用最邻近算法对位图进行缩放
 * @param {Bmp16} &srcBmp
 * @param {int} desiredX
 * @param {int} desiredY
 * @return {*}
 * @Author: Mcfly
 * @Date: 2021-07-04 00:03:06
 */
XinCorePicture::Bmp16 *XinCorePicture::nearestNeighborZoom(Bmp16 &srcBmp, int desiredX, int desiredY)
{
    int w1 = srcBmp.width;
    int h1 = srcBmp.height;
    int w2 = desiredX;
    int h2 = desiredY;
    float xratio = (float)w1 / w2;
    float yratio = (float)h1 / h2;
    float x;
    float y;
    Bmp16 *distBmp = new Bmp16(desiredX, desiredY, new unsigned char[desiredX * desiredY * 2]);

    for (int j = 0; j < w2; j++)
    {
        for (int i = 0; i < h2; i++)
        {
            x = i * xratio;
            y = j * yratio;
            distBmp->setPixel(i, j, srcBmp.getPixel((int)(x + 0.5), (int)(y + 0.5)));
        }
    }

    return distBmp;
}

XinCorePicture::Bmp16 **XinCorePicture::splitBmp16(Bmp16 &srcBmp, int numX, int numY)
{
    int sizeX = srcBmp.width / numX;
    int sizeY = srcBmp.height / numY;
    Bmp16 **bmps = new Bmp16 *[numX * numY];
    for (int j = 0; j < numY; j++)
    {
        for (int i = 0; i < numX; i++)
        {
            unsigned char *splitedBmpData = new unsigned char[sizeX * sizeY * XinUIConstants::BytesPrePixel];
            for (int y = 0; y < sizeY; y++)
            {
                unsigned char *distData = splitedBmpData + y * numX * XinUIConstants::BytesPrePixel;
                unsigned char *srcData = srcBmp.bmpData + i * sizeX * XinUIConstants::BytesPrePixel +
                                         (j * sizeY + y) * srcBmp.width * XinUIConstants::BytesPrePixel;
                memcpy(distData, srcData, sizeX * XinUIConstants::BytesPrePixel);
            }
            bmps[j * numX + i] = new Bmp16(sizeX, sizeY, splitedBmpData);
        }
    }
    return bmps;
}

XinCorePicture::Bmp2 **XinCorePicture::splitBmp2(Bmp2 &srcBmp, int numX, int numY)
{
    int sizeX = srcBmp.width / numX;
    int sizeY = srcBmp.height / numY;
    Bmp2 **bmps = new Bmp2 *[numX * numY];
    for (int j = 0; j < numY; j++)
    {
        for (int i = 0; i < numX; i++)
        {
            unsigned char *splitedBmpData = new unsigned char[sizeX * sizeY];
            int trueWidth = -1, st = sizeX, ed = 0;
            for (int y = 0; y < sizeY; y++)
            {
                unsigned char *distData = splitedBmpData + y * sizeX;
                unsigned char *srcData = srcBmp.bmpData + i * sizeX +
                                         (j * sizeY + y) * srcBmp.width;
                memcpy(distData, srcData, sizeX);
                for (unsigned char *p = distData, k = 0; k < sizeX; k++, p++)
                {
                    if (*p == 1)
                    {
                        if (k < st)
                            st = k;
                        if (k > ed)
                            ed = k;
                    }
                }
            }
            if (ed < sizeX - 1)
                ed++;
            if(st > ed)
            {
                st = 0;
                ed = sizeX - 1;
            }
            trueWidth = ed - st + 1;
            bmps[j * numX + i] = new Bmp2(sizeX, sizeY, trueWidth, st, ed, splitedBmpData);
        }
    }
    return bmps;
}