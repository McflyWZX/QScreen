#ifndef __XIN_CONTROL_H
#define __XIN_CONTROL_H 

class XinPlane
{
private:
    short width;
    short height;
    unsigned char bytesPrePixel;
    unsigned char *vram;

    int coor2Index(unsigned int x, unsigned int y){return y * widthPixels * bytesPrePixel + x;}
public:

};

#endif