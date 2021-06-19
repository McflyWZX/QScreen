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

    int coor2Index(unsigned int x, unsigned int y){return y * widthPixels * bytesPrePixel + x;}
public:
    XinUI();
    int addPlane(); //返回图层id

};

#endif
