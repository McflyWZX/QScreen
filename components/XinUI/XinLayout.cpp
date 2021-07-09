#include "XinLayout.hpp"

/*
 * @Author: Mcfly
 * @Date: 2021-07-09 11:19:07
 * @LastEditTime: 2021-07-09 15:08:31
 * @LastEditors: Mcfly
 * @Description: 
 * @FilePath: \QScreen\components\XinUI\XinLayout.cpp
 * 本人github:https://github.com/McflyWZX
 */
void XinLayout::add(XinControl *ctrl)
{
    if (controls.size())
    {
        switch (layoutMode)
        {
        case LayoutMode::Vertical:
            controls.back()->bottom = ctrl;
            break;
        case LayoutMode::Horizontal:
            controls.back()->right = ctrl;
            break;
        }
    }
    controls.push_back(ctrl);
}
/**
     * @description: 从布局中移去一个控件，同时更新布局中相应控件的连接
     * @param {XinControl} *ctrl
     * @return {*}
     * @Author: Mcfly
     * @Date: 2021-07-09 10:13:52
     */
void XinLayout::remove(XinControl *ctrl)
{
    for (ctrlVector::iterator it = controls.begin(); it != controls.end(); it++)
    {
        if (*it == ctrl)
        {
            controls.erase(it);
            switch (layoutMode)
            {
            case LayoutMode::Vertical:
                if (ctrl->top != NULL)
                    ctrl->top->bottom = ctrl->bottom;
                if (ctrl->bottom != NULL)
                    ctrl->bottom->top = ctrl->top;
                break;
            case LayoutMode::Horizontal:
                if (ctrl->left != NULL)
                    ctrl->left->right = ctrl->right;
                if (ctrl->right != NULL)
                    ctrl->right->left = ctrl->left;
                break;
            }
            return;
        }
    }
}
short XinLayout::getWidth()
{
    short res = 0;
    switch (layoutMode)
    {
    //垂直布局的宽度应该等于所用控件的最大宽度
    case LayoutMode::Vertical:
        for (ctrlVector::iterator it = controls.begin(); it != controls.end(); it++)
        {
            if ((*it)->getWidth() > res)
                res = (*it)->getWidth();
        }
        break;
    //水平布局的宽度应该等于所用控件的宽度和
    case LayoutMode::Horizontal:
        for (ctrlVector::iterator it = controls.begin(); it != controls.end(); it++)
        {
            res += (*it)->getWidth();
        }
        break;
    default:
        break;
    }
    return res;
}
short XinLayout::getHeight()
{
    short res = 0;
    switch (layoutMode)
    {
    //垂直布局的高度应该等于所用控件的高度和
    case LayoutMode::Vertical:
        for (ctrlVector::iterator it = controls.begin(); it != controls.end(); it++)
        {
            res += (*it)->getHeight();
        }
        break;
    //水平布局的高度应该等于所用控件的最大高度
    case LayoutMode::Horizontal:
        for (ctrlVector::iterator it = controls.begin(); it != controls.end(); it++)
        {
            if ((*it)->getHeight() > res)
                res = (*it)->getHeight();
        }
        break;
    default:
        break;
    }
    return res;
}

void XinLayout::draw()
{
    if(!visible)
        return;
    for (ctrlVector::iterator it = controls.begin(); it != controls.end(); it++)
    {
        (*it)->draw();
    }
}