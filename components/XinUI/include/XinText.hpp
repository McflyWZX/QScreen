/*
 * @Author: Mcfly
 * @Date: 2021-06-19 00:47:36
 * @LastEditTime: 2021-07-10 20:19:59
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinText.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_TEXT_H
#define __XIN_TEXT_H

#include "XinUIConstants.hpp"
#include "XinCoreGraphic.hpp"
#include "XinControl.hpp"
#include "string"
class XinText : public XinControl
{
private:
    using XinXY = XinUIConstants::XinXY;
    using string = std::string;

    string txt;
    int txtLen;
    unsigned short color;
    bool autoWarp = true;

    void setHeight();

public:
    XinText(XinXY centerXY, XinXY sizeXY, string s, unsigned short color)
        : XinControl(centerXY, sizeXY), txt(s), txtLen(0), color(color) {
            setHeight();
        }
    void setTxt(string s);
    void draw(unsigned char *vram, XinXY vramSize);
};

#endif