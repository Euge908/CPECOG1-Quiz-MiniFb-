#ifndef STRING
#define STRING
#include <string.h>
#endif

#ifndef STDIO
#define STDIO
#include <stdio.h>
#endif


#ifndef MINIFB
#define MINIFB
#include "include/MiniFB_cpp.h"
#endif

#ifndef FREEIMAGE
#define FREEIMAGE
#include "include/FreeImage.h"
#endif


#ifndef STDINT
#define STDINT
#include<stdint.h>
#endif





#ifndef ENTITY
#define ENTITY
class Entity {
protected:
    //positionX and positionY are relative coordinates to the framebuffer
    int positionX, positionY, width, height, stride, positionYOld, positionXOld, absPositionX, absPositionY, jump, jumpLimit, air, leftState, upState, rightState;
    uint8_t* image_data;
public:
    Entity();

    void printAttributes();


    Entity(int w, int h, FIBITMAP* i);

    void updateOldRelCoords();

    void setAbsX(int x);

    int getAbsX();

    void setAbsY(int y);

    int getAbsY();

    void setXOld(int xold);

    int getXOld();

    void setYOld(int yold);

    int getYOld();

    int getStride();

    void setImageData(FIBITMAP* i);

    uint8_t* getImageData();

    void setPosition(int x, int y);

    int getX();

    int getY();

    void setX(int x);

    void setY(int y);

    void setWidthHeight(int w, int h);

    int getWidth();

    int getHeight();
};

#endif