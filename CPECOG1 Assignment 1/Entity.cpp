#include "Entity.h"

Entity::Entity() {
        positionX = 0;
        positionY = 0;
        width = 0;
        height = 0;
        stride = 0;
        positionYOld = 0;
        positionXOld = 0;
        absPositionX = 0;
        absPositionY = 0;
        jump = 0;
        jumpLimit = 0;
        air = 0;
        leftState = 0;
        upState = 0;
        rightState = 0;

}
void  Entity::printAttributes() {
        //printf("positionX: %d, positionY: %d, width: %d, height: %d, stride: %d, positionYOld: %d, positionXOld: %d, absPositionX: %d, absPositionY: %d\n\n", positionX, positionY, width, height, stride, positionYOld, positionXOld, absPositionX, absPositionY);
}

Entity::Entity(int w, int h, FIBITMAP* i) {
        Entity();
        width = w;
        height = h;
        stride = FreeImage_GetPitch(i);
        setImageData(i);
    }
void Entity::updateOldRelCoords() {
        setYOld(positionY);
        setXOld(positionX);
}

void Entity::setAbsX(int x) {
        absPositionX = x;
}

int Entity::getAbsX() {
        return absPositionX;
}

void Entity::setAbsY(int y) {
        absPositionY = y;
}

int Entity::getAbsY() {
        return absPositionY;
}

void Entity::setXOld(int xold) {
        positionXOld = xold;
}

int Entity::getXOld() {
        return positionXOld;
}

void Entity::setYOld(int yold) {
        positionYOld = yold;
}

int Entity::getYOld() {
        return positionYOld;
}

int Entity::getStride() {
        return stride;
}

void Entity::setImageData(FIBITMAP* i) {
        FreeImage_FlipVertical(i);
        image_data = FreeImage_GetBits(i);
}

uint8_t* Entity::getImageData() {
        return image_data;
}

void Entity::setPosition(int x, int y) {
        positionX = x;
        positionY = y;
}

int Entity::getX() {
        return positionX;
}

int Entity::getY() {
        return positionY;
}

void Entity::setX(int x) {
        positionX = x;
}

void Entity::setY(int y) {
        positionY = y;
}

void Entity::setWidthHeight(int w, int h) {
        width = w;
        height = h;
}

int Entity::getWidth() {
        return width;
}

int Entity::getHeight() {
        return height;
}
