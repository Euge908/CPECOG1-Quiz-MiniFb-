#pragma once
#include "ball.h"
#include "StaticObject.h"
#include "Entity.h"

#define wall_bg_width 2704
#define wall_bg_height 1628

#define window_width 1280
#define window_height 720

#define sprite_width 100
#define sprite_height 100

#define staticObjectsCount 20


typedef struct {
    int bg_x;
    int bg_y;
    int bg_x_old;
    int bg_y_old;
    int width;
    int height;
    int stride;
    uint8_t* img_data;
} backgroundImageHolder;


typedef struct {
    backgroundImageHolder *bg;
    Ball *ball_sprite;
    staticObject *maskObject;
    staticObject *staticObjectList;
    uint32_t *buffer;
    uint8_t *score;
} callbackDataHolder;