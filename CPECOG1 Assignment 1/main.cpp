//fix all //qckui fxi//qcak docktro //languf ftish



//NOTE: Resolve all TODO comments after polishing the code


#include <stdio.h>
#include "include/MiniFB_cpp.h"
#include "include/FreeImage.h"
#include <string.h>

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


class Entity {
protected:
    //positionX and positionY are relative coordinates to the framebuffer
    int positionX, positionY, width, height, stride, positionYOld, positionXOld, absPositionX, absPositionY, jump, jumpLimit, air, leftState, upState, rightState;
    uint8_t* image_data;
public:
    Entity() {
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
    void printAttributes() {
        //printf("positionX: %d, positionY: %d, width: %d, height: %d, stride: %d, positionYOld: %d, positionXOld: %d, absPositionX: %d, absPositionY: %d\n\n", positionX, positionY, width, height, stride, positionYOld, positionXOld, absPositionX, absPositionY);
    }


    Entity(int w, int h, FIBITMAP* i) {
        Entity();
        width = w;
        height = h;
        stride = FreeImage_GetPitch(i);
        setImageData(i);
    }

    void updateOldRelCoords() {
        setYOld(positionY);
        setXOld(positionX);
    }

    void setAbsX(int x) {
        absPositionX = x;
    }

    int getAbsX() {
        return absPositionX;
    }

    void setAbsY(int y) {
        absPositionY = y;
    }

    int getAbsY() {
        return absPositionY;
    }

    void setXOld(int xold) {
        positionXOld = xold;
    }

    int getXOld() {
        return positionXOld;
    }

    void setYOld(int yold) {
        positionYOld = yold;
    }

    int getYOld() {
        return positionYOld;
    }

    int getStride() {
        return stride;
    }

    void setImageData(FIBITMAP* i) {
        FreeImage_FlipVertical(i);
        image_data = FreeImage_GetBits(i);
    }

    uint8_t* getImageData() {
        return image_data;
    }

    void setPosition(int x, int y) {
        positionX = x;
        positionY = y;
    }

    int getX() {
        return positionX;
    }

    int getY() {
        return positionY;
    }

    void setX(int x) {
        positionX = x;
    }

    void setY(int y) {
        positionY = y;
    }

    void setWidthHeight(int w, int h) {
        width = w;
        height = h;
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }
};


class staticObject : public Entity {
protected:
    uint8_t coin, SaveGlass, unpassable, enemy;

public:
    staticObject() : Entity() {
        coin = 0; SaveGlass = 0; unpassable = 0; enemy = 0;
    }

    staticObject(int w, int h, FIBITMAP* i) : Entity(w, h, i) {
        coin = 0;
        SaveGlass = 0;
        unpassable = 0;
        enemy = 0;
    }

    //enemy methods
    uint8_t isEnemy() {
        return enemy;
    }

    uint8_t isEnemy(uint8_t val) {
        enemy = val;
        return enemy;
    }


    //save glass methods
    uint8_t isSaveGlass() {
        return SaveGlass;
    }

    uint8_t isSaveGlass(uint8_t val) {
        SaveGlass = val;
        return SaveGlass;
    }

    //coin methods
    uint8_t isCoin() {
        return coin;
    }

    uint8_t isCoin(uint8_t val) {
        coin = val;
        return coin;
    }

    void setSavePoint(int x, int y) {

    }

    //unpassable obstacle methods
    uint8_t isUnpassable() {
        return unpassable;
    }

    uint8_t isUnpassable(uint8_t val) {
        unpassable = val;
        return unpassable;
    }


};


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

        //[hit from left side of smth OR hit from right side of smth] AND [hit from top of smth OR hit from bottomof smth]



        if (
            ((absPositionX >= smth->getAbsX() && absPositionX <= smth->getAbsX() + smth->getWidth()) ||
                (absPositionX + width >= smth->getAbsX() && absPositionX + width <= smth->getAbsX() + smth->getWidth()))
            &&
            ((absPositionY >= smth->getAbsY() && absPositionY <= smth->getAbsY() + smth->getHeight()) ||
                (absPositionY + height >= smth->getAbsY() && absPositionY + height <= smth->getAbsY() + smth->getHeight()))
            ) {

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


typedef struct {
    backgroundImageHolder* bg;
    Ball* ball_sprite;
    staticObject* maskObject;
    staticObject* staticObjectList;
    uint32_t* buffer;
    uint8_t* score;
} callbackDataHolder;


void updateAbsCoords(Entity* sprite, backgroundImageHolder* bg) {
    //NOTE: Absolute Position is not relative to framebuffer. This function will draw the sprite based from it's absolute position. Specifically, this will transform the absolution position into the framebuffer relative coordinates.

    //convert abs pos to rel pos 
    sprite->setAbsX(sprite->getX() + bg->bg_x);
    sprite->setAbsY(sprite->getY() + bg->bg_y);

}





int getoffsetX(Ball* sprite, backgroundImageHolder* bg) {

    return sprite->getX() - window_width / 2 - sprite->getWidth() / 2; //<0 if it's too far left, and >0 if it's too far right

}


int getoffsetY(Ball* sprite, backgroundImageHolder* bg) {

    return sprite->getY() - window_height / 2 - sprite->getHeight() / 2; //<0 if it's too far up, and >0 if it's too far down

}


//mask is needed pala to daw the walls. I almost forgot
void drawEntity(uint32_t* buffer, Entity* sprite, backgroundImageHolder bg, staticObject* mask) {

    //redraw background at old position of sprite
    // Redraw the background and mask
    for (int i = 0; i < sprite->getHeight(); i++)
    {
        for (int j = 0; j < sprite->getWidth(); j++)
        {
            uint8_t r = mask->getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 2];
            uint8_t g = mask->getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 1];
            uint8_t b = mask->getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j)];;
            uint32_t pixel = (r << 16) | (g << 8) | b;

            //if the pixel at mask is black
            if (!pixel) {
                r = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 2];
                g = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 1];
                b = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j)];
            }

            pixel = (r << 16) | (g << 8) | b;


            buffer[bg.width * (i + bg.bg_y_old + sprite->getYOld()) + (j + sprite->getXOld() + bg.bg_x_old)] = pixel;

        }
    }


    // Draw the sprite
    for (int i = 0; i < sprite->getHeight(); i++)
    {
        for (int j = 0; j < sprite->getWidth(); j++)
        {
            uint8_t r = sprite->getImageData()[sprite->getStride() * i + 3 * j + 2];
            uint8_t g = sprite->getImageData()[sprite->getStride() * i + 3 * j + 1];
            uint8_t b = sprite->getImageData()[sprite->getStride() * i + 3 * j];
            uint32_t pixel = (r << 16) | (g << 8) | b;
            if (pixel)
                buffer[bg.width * (i + sprite->getY() + bg.bg_y) + (j + bg.bg_x + sprite->getX())] = pixel;
        }
    }


}


void unDrawSpriteToBackground(uint32_t* buffer, Entity* sprite, backgroundImageHolder bg, staticObject* mask) {
    // Redraw the background and mask
    for (int i = 0; i < sprite->getHeight(); i++)
    {
        for (int j = 0; j < sprite->getWidth(); j++)
        {
            uint8_t r = mask->getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 2];
            uint8_t g = mask->getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 1];
            uint8_t b = mask->getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j)];;
            uint32_t pixel = (r << 16) | (g << 8) | b;

            //if the pixel at mask is black
            if (!pixel) {
                r = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 2];
                g = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j) + 1];
                b = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite->getYOld()) + 3 * (sprite->getXOld() + bg.bg_x_old + j)];
            }

            pixel = (r << 16) | (g << 8) | b;


            buffer[bg.width * (i + bg.bg_y_old + sprite->getYOld()) + (j + sprite->getXOld() + bg.bg_x_old)] = pixel;

        }
    }
}


uint8_t convertAbstoRelCoords(Entity* sprite, backgroundImageHolder bg) {
    //NOTE: Absolute Position is not relative to framebuffer. This function will draw the sprite based from it's absolute position. Specifically, this will transform the absolution position into the framebuffer relative coordinates.


    //check if abs position of y is within bounds of framebuffer
    if ((sprite->getAbsX() >= bg.bg_x && sprite->getAbsX() + sprite->getWidth() <= bg.bg_x + window_width)
        && (sprite->getAbsY() >= bg.bg_y && sprite->getAbsY() + sprite->getHeight() <= bg.bg_y + window_height)) {

        //convert abs pos to rel pos 
        sprite->setX(sprite->getAbsX() - bg.bg_x);
        sprite->setY(sprite->getAbsY() - bg.bg_y);

        return 1;

    }

    return 0;

}

void drawEntityFromAbsPos(uint32_t* buffer, Entity* sprite, backgroundImageHolder bg, staticObject* mask) {
    if (convertAbstoRelCoords(sprite, bg)) {
        sprite->updateOldRelCoords();

        drawEntity(buffer, sprite, bg, mask);
    }
}


void deleteStaticObject(int index, staticObject* staticObjectList) {
    int i, newIndex = 0;
    for (i = 0; i < staticObjectsCount; i++) {
        if (i != index) {
            staticObjectList[newIndex] = staticObjectList[i];
            newIndex++;
        }
    }
}


//minifb keyboard interrupts prototype
void key_press(struct mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed);

void staticObjectInteraction(callbackDataHolder* callbackData) {

    Ball* ball_sprite = callbackData->ball_sprite;

    backgroundImageHolder* bg = callbackData->bg;

    staticObject* maskObject = callbackData->maskObject;
    staticObject* staticObjectList = callbackData->staticObjectList;
    uint32_t* buffer = callbackData->buffer;

    uint8_t* score = callbackData->score;

    //TODO: for every item in object list that is in the framebuffer, check if collision
    for (int i = 0; i < staticObjectsCount; i++) {
        //draw other entities first
        if (staticObjectList[i].isCoin() || staticObjectList[i].isEnemy() || staticObjectList[i].isSaveGlass() || staticObjectList[i].isUnpassable()) {
            char col = ball_sprite->detectCollision(&staticObjectList[i], *bg);

            if (col == 'c') {
                //if coin collision
                unDrawSpriteToBackground(buffer, &staticObjectList[i], *bg, maskObject);
                deleteStaticObject(i, staticObjectList);

                //TODO: Increment Score
                *score = *score + 1;
            }
            else if (col == 'e') {
                //TODO: implement a die function, where the ball goes to the last save location

                //ball_sprite->setPosition(window_width / 2 - ball_sprite->getWidth() / 2, window_height / 2 - ball_sprite->getHeight() / 2 - 100);
                printf("Character died once");
                ball_sprite->setX(window_width / 2 - ball_sprite->getWidth() / 2);
                ball_sprite->setY(window_height / 2 - ball_sprite->getHeight() / 2 - 100);


                int lifeCount = ball_sprite->getLives() - 1;
                if (lifeCount >= 0) {
                    ball_sprite->setLives(lifeCount);
                }
            }
        }

    }


}


//convertAbstoRelCoords(Entity* sprite, backgroundImageHolder bg)
void displayStaticScoreLife(uint32_t* buffer, staticObject* lifeList, staticObject* scoreList, backgroundImageHolder bg, staticObject* mask, uint8_t score, uint8_t life)
{
    FIBITMAP* fi_num0 = FreeImage_Load(FIF_PNG, "assets/num0.png");
    FIBITMAP* fi_num1 = FreeImage_Load(FIF_PNG, "assets/num1.png");
    FIBITMAP* fi_num2 = FreeImage_Load(FIF_PNG, "assets/num2.png");
    FIBITMAP* fi_num3 = FreeImage_Load(FIF_PNG, "assets/num3.png");
    FIBITMAP* fi_num4 = FreeImage_Load(FIF_PNG, "assets/num4.png");
    FIBITMAP* fi_num5 = FreeImage_Load(FIF_PNG, "assets/num5.png");
    FIBITMAP* fi_num6 = FreeImage_Load(FIF_PNG, "assets/num6.png");
    FIBITMAP* fi_num7 = FreeImage_Load(FIF_PNG, "assets/num7.png");
    FIBITMAP* fi_num8 = FreeImage_Load(FIF_PNG, "assets/num8.png");
    FIBITMAP* fi_num9 = FreeImage_Load(FIF_PNG, "assets/num9.png");



    //score is only two digits
    //life is only two digits
    //TODO: Make condition so that it doesn't overflow
    score %= 100;
    life %= 100;


    uint8_t scoreOnes = score % 10;



    uint8_t scoreTens = (uint8_t)score / 10;

    uint8_t lifeOnes = life % 10;
    uint8_t lifeTens = life / 10;

    switch (lifeOnes) {
    case 0:
        lifeList[2].setImageData(fi_num0);
        break;
    case 1:
        lifeList[2].setImageData(fi_num1);
        break;
    case 2:
        lifeList[2].setImageData(fi_num2);
        break;
    case 3:
        lifeList[2].setImageData(fi_num3);
        break;
    case 4:
        lifeList[2].setImageData(fi_num4);
        break;
    case 5:
        lifeList[2].setImageData(fi_num5);
        break;
    case 6:
        lifeList[2].setImageData(fi_num6);
        break;
    case 7:
        lifeList[2].setImageData(fi_num7);
        break;
    case 8:
        lifeList[2].setImageData(fi_num8);
        break;
    case 9:
        lifeList[2].setImageData(fi_num9);
        break;

    }

    switch (lifeTens) {
    case 0:
        lifeList[1].setImageData(fi_num0);
        break;
    case 1:
        lifeList[1].setImageData(fi_num1);
        break;
    case 2:
        lifeList[1].setImageData(fi_num2);
        break;
    case 3:
        lifeList[1].setImageData(fi_num3);
        break;
    case 4:
        lifeList[1].setImageData(fi_num4);
        break;
    case 5:
        lifeList[1].setImageData(fi_num5);
        break;
    case 6:
        lifeList[1].setImageData(fi_num6);
        break;
    case 7:
        lifeList[1].setImageData(fi_num7);
        break;
    case 8:
        lifeList[1].setImageData(fi_num8);
        break;
    case 9:
        lifeList[1].setImageData(fi_num9);
        break;

    }

    switch (scoreOnes) {
    case 0:
        scoreList[2].setImageData(fi_num0);
        break;
    case 1:
        scoreList[2].setImageData(fi_num1);
        break;
    case 2:
        scoreList[2].setImageData(fi_num2);
        break;
    case 3:
        scoreList[2].setImageData(fi_num3);
        break;
    case 4:
        scoreList[2].setImageData(fi_num4);
        break;
    case 5:
        scoreList[2].setImageData(fi_num5);
        break;
    case 6:
        scoreList[2].setImageData(fi_num6);
        break;
    case 7:
        scoreList[2].setImageData(fi_num7);
        break;
    case 8:
        scoreList[2].setImageData(fi_num8);
        break;
    case 9:
        scoreList[2].setImageData(fi_num9);
        break;

    }

    switch (scoreTens) {
    case 0:
        scoreList[1].setImageData(fi_num0);
        break;
    case 1:
        scoreList[1].setImageData(fi_num1);
        break;
    case 2:
        scoreList[1].setImageData(fi_num2);
        break;
    case 3:
        scoreList[1].setImageData(fi_num3);
        break;
    case 4:
        scoreList[1].setImageData(fi_num4);
        break;
    case 5:
        scoreList[1].setImageData(fi_num5);
        break;
    case 6:
        scoreList[1].setImageData(fi_num6);
        break;
    case 7:
        scoreList[1].setImageData(fi_num7);
        break;
    case 8:
        scoreList[1].setImageData(fi_num8);
        break;
    case 9:
        scoreList[1].setImageData(fi_num9);
        break;

    }

    //TODO: Add conditions to change value of stuff here
    for (int j = 0; j < 3; j++) {

        drawEntity(buffer, &lifeList[j], bg, mask);
    }

    for (int i = 0; i < 3; i++) {
        drawEntity(buffer, &scoreList[i], bg, mask);
    }
}


int main()
{
    struct mfb_window* window = mfb_open_ex("my display", window_width, window_height, WF_RESIZABLE);
    if (!window)
        return 0;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Variable Declarations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    uint8_t paused = 0, gameOver = 0;
    uint8_t score = 0;

    //TODO: replace this with enemy object list instead and add staticObjectList
    //Entity* entityList = (Entity*)malloc(20 * sizeof(Entity)); //20 items by default 
    // 
    // 
    staticObject* staticObjectList = new staticObject[staticObjectsCount]; //20 items by default





    //create an oversized framebuffer
    uint32_t* buffer = (uint32_t*)malloc(wall_bg_width * wall_bg_height * 4);
    FIBITMAP* fi_bg = FreeImage_Load(FIF_PNG, "assets/testBackground.png");
    FreeImage_FlipVertical(fi_bg);
    uint8_t* bg = FreeImage_GetBits(fi_bg);

    //BALL CLASS AND SPRITE
    FIBITMAP* fi_ball = FreeImage_Load(FIF_PNG, "assets/pepe_ball.png");
    Ball ball_sprite = Ball(sprite_width, sprite_height, fi_ball);
    ball_sprite.setPosition(window_width / 2 - ball_sprite.getWidth() / 2, window_height / 2 - ball_sprite.getHeight() / 2 - 100);

    ball_sprite.setYOld(ball_sprite.getY());
    ball_sprite.setXOld(ball_sprite.getX());
    ball_sprite.setLives(3);


    //BACKGROUND IMG STUFF
    backgroundImageHolder bg_img;
    bg_img.bg_x = 0;
    bg_img.bg_x_old = 0;
    bg_img.bg_y = 500;
    bg_img.bg_y_old = 0;
    bg_img.img_data = bg;
    bg_img.width = wall_bg_width;
    bg_img.height = wall_bg_height;

    //MASK OBJECT AND SPRITE
    FIBITMAP* fi_mask = FreeImage_Load(FIF_PNG, "assets/firstFloor.png");
    staticObject maskObject = staticObject(2704, 1628, fi_mask);
    maskObject.isUnpassable(1);


    //COIN STUFF ADD (ASSUME COINS ARE FLOATING)
    FIBITMAP* fi_coin = FreeImage_Load(FIF_PNG, "assets/coin.png");
    FIBITMAP* fi_enemy = FreeImage_Load(FIF_PNG, "assets/sampleEnemy2.png");




    //TODO: put this in a function instead
    staticObjectList[0] = staticObject(80, 80, fi_coin);
    staticObjectList[0].isCoin(1);
    staticObjectList[0].setAbsX(300);
    staticObjectList[0].setAbsY(200);
    convertAbstoRelCoords(&staticObjectList[0], bg_img);


    staticObjectList[1] = staticObject(50, 50, fi_coin);
    staticObjectList[1].isCoin(1);
    staticObjectList[1].setAbsX(1300);
    staticObjectList[1].setAbsY(647);
    convertAbstoRelCoords(&staticObjectList[1], bg_img);


    staticObjectList[2] = staticObject(40, 100, fi_enemy);
    staticObjectList[2].isEnemy(1);
    staticObjectList[2].setAbsX(1001);
    staticObjectList[2].setAbsY(827);
    convertAbstoRelCoords(&staticObjectList[2], bg_img);



    staticObjectList[3] = staticObject(50, 50, fi_coin);
    staticObjectList[3].isCoin(1);
    staticObjectList[3].setAbsX(1400);
    staticObjectList[3].setAbsY(647);
    convertAbstoRelCoords(&staticObjectList[3], bg_img);



    staticObjectList[4] = staticObject(50, 50, fi_coin);
    staticObjectList[4].isCoin(1);
    staticObjectList[4].setAbsX(1500);
    staticObjectList[4].setAbsY(647);
    convertAbstoRelCoords(&staticObjectList[4], bg_img);


    //BALL GRAVITY
    int gravity = 2;

    // Copies the full background to the framebuffer
    for (int i = 0; i < bg_img.width * bg_img.height * 3; i += 3) {
        buffer[i / 3] = (bg[i + 2] << 16) | (bg[i + 1] << 8) | (bg[i]);
    }

    for (int i = 0; i < maskObject.getWidth() * maskObject.getHeight() * 3; i += 3) {
        uint32_t pixel = (maskObject.getImageData()[i + 2] << 16) | (maskObject.getImageData()[i + 1] << 8) | (maskObject.getImageData()[i]);
        if (pixel) {
            buffer[i / 3] = pixel;
        }
    }

    //DATA TO BE PASSED TO CALLBACK
    callbackDataHolder callbackData;
    callbackData.ball_sprite = &ball_sprite;
    callbackData.bg = &bg_img;
    callbackData.maskObject = &maskObject;
    callbackData.staticObjectList = staticObjectList;
    callbackData.buffer = buffer;
    callbackData.score = &score;



    //Initialize score and life 
    staticObject* scoreList = new staticObject[3];
    staticObject* lifeList = new staticObject[3];

    //score text
    FIBITMAP* fi_score_text = FreeImage_Load(FIF_PNG, "assets/ScoreFinal.png");
    FIBITMAP* fi_zero = FreeImage_Load(FIF_PNG, "assets/num0.png");

    scoreList[0] = staticObject(50, 50, fi_score_text);
    scoreList[0].setX(1099);
    scoreList[0].setY(0);
    scoreList[0].setXOld(1099);
    scoreList[0].setYOld(0);


    scoreList[1] = staticObject(50, 50, fi_zero);
    scoreList[1].setX(1149);
    scoreList[1].setY(0);
    scoreList[1].setXOld(1149);
    scoreList[1].setYOld(0);


    scoreList[2] = staticObject(50, 50, fi_zero);
    scoreList[2].setX(1199);
    scoreList[2].setY(0);
    scoreList[2].setXOld(1199);
    scoreList[2].setYOld(0);



    //life text

    FIBITMAP* fi_life_text = FreeImage_Load(FIF_PNG, "assets/bigLife.png");
    lifeList[0] = staticObject(189, 46, fi_life_text);
    lifeList[0].setX(0);
    lifeList[0].setY(0);
    lifeList[0].setXOld(0);
    lifeList[0].setYOld(0);


    lifeList[1] = staticObject(50, 50, fi_zero);
    lifeList[1].setX(189);
    lifeList[1].setY(0);
    lifeList[1].setXOld(189);
    lifeList[1].setYOld(0);

    lifeList[2] = staticObject(50, 50, fi_zero);
    lifeList[2].setX(239);
    lifeList[2].setY(0);
    lifeList[2].setXOld(239);
    lifeList[2].setYOld(0);


    //INITIALIZE KEYBOARD INTERRUPTS
    mfb_set_keyboard_callback(window, key_press);
    mfb_set_user_data(window, (void*)&callbackData);



    do {

        //* TODO : UNCOMMENT THIS TO ENABLE GRAVITY
        ball_sprite.setY(ball_sprite.getY() + gravity);
        //if there is collision reverse this move
        if (ball_sprite.detectCollision(&maskObject, bg_img)){
            ball_sprite.setY(ball_sprite.getY() - gravity);
            ball_sprite.setAir(0);        
        }
        

        if (ball_sprite.getJump() > 0) {
            
            if (ball_sprite.getRightState()) {
                ball_sprite.testMoveX(5);
                if (ball_sprite.detectCollision(&maskObject, bg_img)) {
                    ball_sprite.testMoveX(-5);

                }
                
            }
            else if (ball_sprite.getLeftState()) {
                ball_sprite.testMoveX(-5);
                if (ball_sprite.detectCollision(&maskObject, bg_img)) {
                    ball_sprite.testMoveX(5);

                }
            }


            ball_sprite.setY(ball_sprite.getY() - 5);

            ball_sprite.setJump(ball_sprite.getJump() - 1);


            if (ball_sprite.detectCollision(&maskObject, bg_img)) {
                ball_sprite.testMoveY(5);
            }

        
        }
        else {
            if (ball_sprite.getRightState()) {
                ball_sprite.testMoveX(5);
                ball_sprite.setRightState(0);
                if (ball_sprite.detectCollision(&maskObject, bg_img)) {
                    ball_sprite.testMoveX(-5);

                }
            }
            else if (ball_sprite.getLeftState()) {
                ball_sprite.testMoveX(-5);
                ball_sprite.setLeftState(0);
                if (ball_sprite.detectCollision(&maskObject, bg_img)) {
                    ball_sprite.testMoveX(5);

                }
            }
        }


        for (int i = 0; i < staticObjectsCount; i++) {
            //draw other entities first
            if (staticObjectList[i].isCoin() || staticObjectList[i].isEnemy() || staticObjectList[i].isSaveGlass() || staticObjectList[i].isUnpassable()) {
                drawEntityFromAbsPos(buffer, &staticObjectList[i], bg_img, &maskObject);
            }

        }

        updateAbsCoords(&ball_sprite, &bg_img);

        
        //printf("\nBall Sprite: %d\n", ball_sprite.getJumpLimit());
        //if (ball_sprite.getJumpLimit() >= 100) {
        //    ball_sprite.setJumpLimit(-100);
        //}


        for (int i = 0; i < staticObjectsCount; i++) {
            //draw other entities first
            if (staticObjectList[i].isCoin() || staticObjectList[i].isEnemy() || staticObjectList[i].isSaveGlass() || staticObjectList[i].isUnpassable()) {
                drawEntityFromAbsPos(buffer, &staticObjectList[i], bg_img, &maskObject);
            }

        }


        printf("Life: %d\n", ball_sprite.getLives());
        displayStaticScoreLife(buffer, lifeList, scoreList, bg_img, &maskObject, score, ball_sprite.getLives());








        updateAbsCoords(&ball_sprite, &bg_img);



        //draw ball last
        drawEntity(buffer, &ball_sprite, bg_img, &maskObject);


        //TODO: Reenable this to print stuff
        //printf("score: %d\n", score);
        //printf("life: %d\n", ball_sprite.getLives());

        //set old coordinates
        ball_sprite.updateOldRelCoords();

        for (int i = 0; i < staticObjectsCount; i++) {
            //draw other entities first
            if (staticObjectList[i].isCoin() || staticObjectList[i].isEnemy() || staticObjectList[i].isSaveGlass() || staticObjectList[i].isUnpassable()) {
                staticObjectList[i].updateOldRelCoords();
            }

        }


        staticObjectInteraction(&callbackData);

        bg_img.bg_x_old = bg_img.bg_x;
        bg_img.bg_y_old = bg_img.bg_y;

        int state = mfb_update_crop(window, buffer + bg_img.bg_y * bg_img.width + bg_img.bg_x, window_width, window_height, bg_img.width);

        if (state < 0) {
            window = NULL;
            break;
        }
    } while (mfb_wait_sync(window));

    //TODO: Free every pointer here
    return 0;
}




void key_press(struct mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed) {

    callbackDataHolder* callbackData = (callbackDataHolder*)mfb_get_user_data(window);
    Ball* ball_sprite = callbackData->ball_sprite;

    backgroundImageHolder* bg = callbackData->bg;

    staticObject* maskObject = callbackData->maskObject;
    staticObject* staticObjectList = callbackData->staticObjectList;
    uint32_t* buffer = callbackData->buffer;

    uint8_t* score = callbackData->score;


    int offsetX = getoffsetX(ball_sprite, bg);
    int offsetY = getoffsetY(ball_sprite, bg);


    printf("offx: %d, offy: %d\n", offsetX, offsetY);

    if (isPressed) {
        //TODO: add conditions to detect collsions and prevent ball from going beyond the window borders
        if (key == KB_KEY_LEFT) {

            ball_sprite->setLeftState(1);
            ball_sprite->setRightState(0);


            if (bg->bg_x - 10 >= 0 && bg->bg_x <= bg->width - window_width) {
                bg->bg_x -= 10;
                if (ball_sprite->detectCollision(maskObject, *bg)) {
                    bg->bg_x += 10;
                }
            }
            else if (ball_sprite->getX() - 10 >= 0) {

                ball_sprite->testMoveX(-10);
                if (ball_sprite->detectCollision(maskObject, *bg)) {
                    ball_sprite->testMoveX(10);
                }
            }
        } //endif left
        if (key == KB_KEY_RIGHT) {
            ball_sprite->setRightState(1);
            ball_sprite->setLeftState(0);
            if (bg->bg_x + 10 <= bg->width - window_width && bg->bg_x + window_width <= bg->width) {
                bg->bg_x += 10;
                if (ball_sprite->detectCollision(maskObject, *bg)) {
                    bg->bg_x -= 10;
                }

            }
            else if (ball_sprite->getX() + 10 <= window_width - ball_sprite->getWidth()) {
                ball_sprite->testMoveX(10);
                if (ball_sprite->detectCollision(maskObject, *bg)) {
                    ball_sprite->testMoveX(-10);
                }
            }
        } //endif right
        if (key == KB_KEY_UP && !ball_sprite->getJump() && !ball_sprite->getAir()) {
            ball_sprite->setJump(50);
            ball_sprite->setAir(1);

            //if (bg->bg_y - 20 >= 0 && bg->bg_y <= bg->height - window_height) {
            //    bg->bg_y -= 20;
            //    if (ball_sprite->detectCollision(maskObject, *bg)) {
            //        bg->bg_y += 20;
            //    }

            //}
            if (ball_sprite->getY() - 20 >= 0) {
                ball_sprite->testMoveY(-20);
                if (ball_sprite->detectCollision(maskObject, *bg)) {
                    ball_sprite->testMoveY(20);
                }


            }

        } //endif up

        else if (key == KB_KEY_DOWN) {
            ball_sprite->setLeftState(0);
            ball_sprite->setRightState(0);
        } //endif down

        staticObjectInteraction(callbackData);
    }
    else {

        //ball_sprite.setY(ball_sprite.getY() - 5);
        //ball_sprite.setJump(ball_sprite.getJump() - 1);


        if (key == KB_KEY_LEFT) {

            ball_sprite->setLeftState(0);

        } //endif left
        if (key == KB_KEY_RIGHT) {
            ball_sprite->setRightState(0);
        } //endif right


    }
}
