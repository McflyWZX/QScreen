/*
 * @Author: Mcfly
 * @Date: 2021-07-02 22:42:31
 * @LastEditTime: 2021-07-03 00:31:19
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinCore\XinCorePicture.cpp
 * 本人github:https://github.com/McflyWZX
 */
/*
 * @Author: Mcfly
 * @Date: 2021-07-02 22:42:31
 * @LastEditTime: 2021-07-02 22:42:41
 * @LastEditors: Mcfly
 * @Description: 提供XinOS的图片处理内核
 * @FilePath: \QScreen\components\XinCore\XinCorePicture.cpp
 * 本人github:https://github.com/McflyWZX
 */

#include "XinCorePicture.hpp"

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
XinCorePicture::Bmp16* XinCorePicture::bmp24Raw2Bmp16(Bmp24Raw &bmpRaw)
{
    //分配存储bmp16色彩数据的内存
    unsigned char *bmp16Data = new unsigned char[bmpRaw.width * bmpRaw.height * 2];

    unsigned char *rowStart, *rowEnd;
    unsigned short *bmp16Color = (unsigned short *)bmp16Data; //以16位的方式填写数据，效率更高
    //因为历史原因bmp图像y轴是倒着的，故倒过来读取，但是当height为负数时就是正着的，一般不会
    for(int y = bmpRaw.height - 1; y >= 0; y--)
    {
        rowStart = bmpRaw.bmpData + y * bmpRaw.rowSize;
        rowEnd = rowStart + bmpRaw.width * 3;
        for(unsigned char *xColor = rowStart; xColor < rowEnd; xColor += 3)
        {
            //迭代每一个RGB并把它写入16位系统位图数据中
            *(bmp16Color++) = RGB24TORGB16(*(xColor + 2), *(xColor + 1), *xColor);
        }
    }

    return new Bmp16(bmpRaw.width, bmpRaw.height, bmp16Data);
}