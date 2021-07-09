/*
 * @Author: Mcfly
 * @Date: 2021-06-19 00:49:36
 * @LastEditTime: 2021-07-09 14:31:47
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\include\XinBar.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_BAR_H
#define __XIN_BAR_H

#include "XinControl.hpp"

class XinBar : public XinControl
{
    using XinUIConstants::XinXY;
private:
    XinLayout barLayout;

public:
    XinBar(XinXY pos, XinXY size) : XinControl(pos, size), 66;
};

#endif