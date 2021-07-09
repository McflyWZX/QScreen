/*
 * @Author: Mcfly
 * @Date: 2021-07-08 19:42:54
 * @LastEditTime: 2021-07-09 15:04:08
 * @LastEditors: Mcfly
 * @Description: 提供单向布局的对象
 * @FilePath: \QScreen\components\XinUI\include\XinLayout.hpp
 * 本人github:https://github.com/McflyWZX
 */
#ifndef __XIN_LAYOUT_H
#define __XIN_LAYOUT_H

#include "vector"
#include "XinControl.hpp"
#include "XinUIConstants.hpp"

class XinLayout : public XinControl
{
public:
    //垂直、水平
    enum LayoutMode
    {
        Vertical,
        Horizontal
    };
    enum AlignmentMode
    {
        Right,
        Center,
        Left
    };
private:
    using ctrlVector = std::vector<XinControl *>;
    using XinXY = XinUIConstants::XinXY;
    
    LayoutMode layoutMode;
    AlignmentMode alignmentMode;
    ctrlVector controls;
public:
    XinLayout(XinXY centerXY, XinXY sizeXY, LayoutMode layoutMode, AlignmentMode alignmentMode)
        : XinControl(centerXY, sizeXY), layoutMode(layoutMode), alignmentMode(alignmentMode) {}
    /**
     * @description: 从布局中填加一个控件，同时更新布局中相应控件的连接
     * @param {XinControl} *ctrl
     * @return {*}
     * @Author: Mcfly
     * @Date: 2021-07-09 10:15:15
     */
    void add(XinControl *ctrl);
    /**
     * @description: 从布局中移去一个控件，同时更新布局中相应控件的连接
     * @param {XinControl} *ctrl
     * @return {*}
     * @Author: Mcfly
     * @Date: 2021-07-09 10:13:52
     */
    void remove(XinControl *ctrl);
    short getWidth();
    short getHeight();
    void draw();
};

#endif