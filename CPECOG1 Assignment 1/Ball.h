#pragma once
#include "Entity.h"
#include "StaticObject.h"
#include "DrawingRelated.h"


class Ball : public Entity {
private:
    int powerUpState; //ball size
    uint8_t lives;
    int direction;
    int state;
    int movementSpeed;
    uint8_t saveX, saveY;

    uint8_t dead;
    uint8_t moving;
    uint8_t hitEnemy;


public:
    Ball(int w, int h, FIBITMAP* i) : Entity(w, h, i) {
        powerUpState = 0;
        lives = 0;
        direction = 0;
        state = 0;
        movementSpeed = 0;
        dead = 0;
        moving = 0;
        hitEnemy = 0;
        saveX = 0; saveY = 0;
    }

    uint8_t getLives() {
        return lives;
    }

    void setLives(uint8_t val) {
        lives = val;
    }


    void move() {
        if (direction) {
            positionX += movementSpeed;
        }
        else {
            positionX -= movementSpeed;
        }
    }

    void testMoveX(int step) {
        positionX += step;
    }

    void testMoveY(int step) {
        positionY += step;

    }


    char detectCollision(staticObject* smth, backgroundImageHolder bg) {
        // 
        // 
        //[hit from left side of smth OR hit from right side of smth] AND [hit from top of smth OR hit from bottomof smth]
        if (
            ((absPositionX >= smth->getAbsX() && absPositionX <= smth->getAbsX() + smth->getWidth()) ||
                (absPositionX + width >= smth->getAbsX() && absPositionX + width <= smth->getAbsX() + smth->getWidth()))
            &&
            ((absPositionY >= smth->getAbsY() && absPositionY <= smth->getAbsY() + smth->getHeight()) ||
                (absPositionY + height >= smth->getAbsY() && absPositionY + height <= smth->getAbsY() + smth->getHeight()))
            ) {

            //TODO: this code assumes that the hitbox is a literal box (not a circle) for the ball


            if (smth->isCoin()) {
                return 'c';
            }
            else if (smth->isSaveGlass()) {
                return 's';
            }
            else if (smth->isUnpassable()) {
                //if topleft is in collision
                uint8_t r = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x) + 2];
                uint8_t g = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x) + 1];
                uint8_t b = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x)];

                uint32_t pixel = (r << 16) | (g << 8) | b;
                if (pixel) {
                    return 'u';
                }

                //if topright is in collision
                r = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x + width) + 2];
                g = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x + width) + 1];
                b = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x + width)];

                pixel = (r << 16) | (g << 8) | b;
                if (pixel) {
                    return 'u';
                }

                //if bottomleft is in collision
                r = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x) + 2];
                g = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x) + 1];
                b = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x)];

                pixel = (r << 16) | (g << 8) | b;
                if (pixel) {
                    return 'u';

                }


                //if bottomright is in collision
                r = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x + width) + 2];
                g = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x + width) + 1];
                b = smth->getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x + width)];

                pixel = (r << 16) | (g << 8) | b;
                if (pixel) {
                    return 'u';
                }
            }
            else if (smth->isEnemy()) {
                return 'e';
            }


        }


        return 0;
    }

    void setJump(int x) {
        jump = x;
    }

    void setJumpLimit(int x) {
        jumpLimit = x;
    }

    void setAir(int x) {
        air = x;
    }

    void setUpState(int x) {
        upState = x;
    }

    void setLeftState(int x) {
        leftState = x;
    }

    void setRightState(int x) {
        rightState = x;
    }

    uint8_t getLeftState() {
        return leftState;
    }

    uint8_t getUpState() {
        return upState;
    }

    uint8_t getRightState() {
        return rightState;
    }

    uint8_t getJump() {
        return jump;
    }

    uint8_t getJumpLimit() {
        return jumpLimit;
    }

    uint8_t getAir() {
        return air;
    }

    uint8_t onFloor() {
        //TODO: smth about callbacks
        jump = 0;
        air = 0;
        return jump;
    }

    uint8_t isDead() {
        return dead;
    }

    uint8_t isDead(uint8_t val) {
        dead = val;
        return dead;
    }

    uint8_t isBig() {
        return 0;
    }

    void updateSize(int s) {
        powerUpState = s;
    }

    void captureFlag() {
    }

    void DieByFalling() {

    }

    void DieByObstacle() {

    }

    void setPowerUp() {

    }

};