//NOTE: Resolve all TODO comments after polishing the code


#include <stdio.h>
#include "include/MiniFB_cpp.h"
#include "include/FreeImage.h"
#include <string.h>

#define wall_bg_width 2704
#define wall_bg_height 1628

#define window_width 800
#define window_height 600

#define sprite_width 100
#define sprite_height 100


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
    int positionX, positionY, width, height, stride, positionYOld, positionXOld, absPositionX, absPositionY;
    uint8_t* image_data;
public:
    Entity() {
        positionX = 0;
        positionY = 0;
        width = 0;
        height = 0;
        stride = 0;

    }

    Entity(int w, int h, FIBITMAP* i) {
        Entity();
        width = w;
        height = h;
        stride = FreeImage_GetPitch(i);
        setImageData(i);
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


class staticObject: public Entity {
protected:
    uint8_t coin, SaveGlass, unpassable;

public:
    staticObject() {

    }

    staticObject(int w, int h, FIBITMAP* i) : Entity(w, h, i) {
        coin = 0;
        SaveGlass = 0;
        unpassable = 0;
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


class Ball: public Entity{
private:
    int powerUpState; //ball size
    int lives;
    int direction;
    int state;
    int movementSpeed;

    uint8_t dead;
    uint8_t moving;
    uint8_t hitEnemy;



public:
    Ball(int w, int h, FIBITMAP* i): Entity(w, h, i) {
        powerUpState = 0; 
        lives = 0;
        direction = 0;
        state = 0;
        movementSpeed = 0;
        dead = 0;
        moving = 0;
        hitEnemy = 0;
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


    uint8_t detectCollision(staticObject smth, backgroundImageHolder bg) {
        //[hit from left side of smth OR hit from right side of smth] AND [hit from top of smth OR hit from bottomof smth]
        if ( 
            (( positionX >= smth.getX() && positionX <= smth.getX() + smth.getWidth()) || 
                (positionX + width > smth.getX() && positionX + width < smth.getX() + smth.getWidth() ))
            &&
            (( positionY >= smth.getY() && positionY <= smth.getY() + smth.getHeight()) || 
                (positionY + height >= smth.getY() && positionY + height <= smth.getY() + smth.getHeight() ))
        ){
            uint8_t col = 0;

            //TODO: this code assumes that the hitbox is a literal box (not a circle) for the ball
            // 
            //if topleft is in collision
            uint8_t r = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x) + 2];
            uint8_t g = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x) + 1];
            uint8_t b = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x)];

            uint32_t pixel = (r << 16) | (g << 8) | b;
            if (pixel) {
                col = 1;
            }

            //if topright is in collision
            r = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x + width) + 2];
            g = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x + width) + 1];
            b = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY()) + 3 * (getX() + bg.bg_x + width)];

            pixel = (r << 16) | (g << 8) | b;
            if (pixel) {
                col = 1;
            }

            //if bottomleft is in collision
            r = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x) + 2];
            g = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x) + 1];
            b = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x)];

            pixel = (r << 16) | (g << 8) | b;
            if (pixel) {
                col = 1;
            }


            //if bottomright is in collision
            r = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x + width) + 2];
            g = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x + width) + 1];
            b = smth.getImageData()[bg.width * 3 * (bg.bg_y + getY() + height) + 3 * (getX() + bg.bg_x + width)];

            pixel = (r << 16) | (g << 8) | b;
            if (pixel) {
                col = 1;
            }



            if (col) {
                if (smth.isCoin()) {
                    return 'c';
                }
                else if (smth.isSaveGlass()) {
                    return 's';
                }
                else if (smth.isUnpassable()) {
                    return 'u';
                }
            }
            else {
                return 0;
            }

        }
        

        return 0;
    }

    uint8_t isJumping() {
        //TODO: smth about callbacks
        return 0;
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

void sampleLevel() {

}

typedef struct {
    backgroundImageHolder* bg;
    Ball* ball_sprite;
    staticObject* maskObject;
} callbackDataHolder;


//TODO: add more levels
void levelOne();
void levelTwo();


void drawEntityFromAbsolutePosition(uint32_t* buffer, Entity sprite, backgroundImageHolder bg, staticObject mask) {
    //NOTE: Absolute Position is relative to framebuffer
}

void drawEntity(uint32_t* buffer, Entity sprite, backgroundImageHolder bg, staticObject mask) {
    //redraw background at old position of sprite
    // Redraw the background and mask
    for (int i = 0; i < sprite.getHeight(); i++)
    {
        for (int j = 0; j < sprite.getWidth(); j++)
        {
            uint8_t r = mask.getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite.getYOld()) + 3 * (sprite.getXOld() + bg.bg_x_old + j) + 2];
            uint8_t g = mask.getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite.getYOld()) + 3 * (sprite.getXOld() + bg.bg_x_old + j) + 1];
            uint8_t b = mask.getImageData()[bg.width * 3 * (i + bg.bg_y_old + sprite.getYOld()) + 3 * (sprite.getXOld() + bg.bg_x_old + j)];;
            uint32_t pixel = (r << 16) | (g << 8) | b;
            
            //if the pixel at mask is black
            if (!pixel) {
                r = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite.getYOld()) + 3 * (sprite.getXOld() + bg.bg_x_old + j) + 2];
                g = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite.getYOld()) + 3 * (sprite.getXOld() + bg.bg_x_old + j) + 1];
                b = bg.img_data[bg.width * 3 * (i + bg.bg_y_old + sprite.getYOld()) + 3 * (sprite.getXOld() + bg.bg_x_old + j)];
            }

            pixel = (r << 16) | (g << 8) | b;

            
            buffer[bg.width * (i + bg.bg_y_old + sprite.getYOld()) + (j + sprite.getXOld() + bg.bg_x_old)] = pixel;

        }
    }


    // Draw the sprite
    for (int i = 0; i < sprite.getHeight(); i++)
    {
        for (int j = 0; j < sprite.getWidth(); j++)
        {
            uint8_t r = sprite.getImageData()[sprite.getStride() * i + 3 * j + 2];
            uint8_t g = sprite.getImageData()[sprite.getStride() * i + 3 * j + 1];
            uint8_t b = sprite.getImageData()[sprite.getStride() * i + 3 * j];
            uint32_t pixel = (r << 16) | (g << 8) | b;
            if (pixel)
                buffer[bg.width * (i + sprite.getY() + bg.bg_y) + (j + bg.bg_x + sprite.getX())] = pixel;
        }
    }
}

//minifb keyboard interrupts prototype
void key_press(struct mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed);

int main()
{
    struct mfb_window* window = mfb_open_ex("my display", window_width, window_height, WF_RESIZABLE);
    if (!window)
        return 0;



    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Variable Declarations~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    uint8_t paused = 0, gameOver = 0;
    int score = 0;

    //TODO: replace this with enemy object list instead and add staticObjectList
    //Entity* entityList = (Entity*)malloc(20 * sizeof(Entity)); //20 items by default 
    // 
    // 
    staticObject *staticObjectList = new staticObject[20]; //20 items by default

    //create an oversized framebuffer
    uint32_t* buffer = (uint32_t*)malloc(wall_bg_width * wall_bg_height * 4);
    FIBITMAP* fi_bg = FreeImage_Load(FIF_PNG, "assets/testBackground.png");
    FreeImage_FlipVertical(fi_bg);
    uint8_t* bg = FreeImage_GetBits(fi_bg);

    //BALL CLASS AND SPRITE
    FIBITMAP* fi_ball = FreeImage_Load(FIF_PNG, "assets/sprite.png"); 
    Ball ball_sprite = Ball(sprite_width, sprite_height, fi_ball);
    ball_sprite.setPosition(window_width / 2 - ball_sprite.getWidth() / 2, window_height / 2 - ball_sprite.getHeight() / 2);

    ball_sprite.setYOld(ball_sprite.getY());
    ball_sprite.setXOld(ball_sprite.getX());

    //BACKGROUND IMG STUFF
    backgroundImageHolder bg_img;
    bg_img.bg_x = 0;
    bg_img.bg_x_old = 0;
    bg_img.bg_y = 0;
    bg_img.bg_y_old = 0;
    bg_img.img_data = bg;
    bg_img.width = wall_bg_width;
    bg_img.height = wall_bg_height;

    //MASK OBJECT AND SPRITE
    FIBITMAP* fi_mask = FreeImage_Load(FIF_PNG, "assets/testMask.png");
    staticObject maskObject = staticObject(2704, 1628, fi_mask);
    maskObject.isUnpassable(1);


    //COIN STUFF ADD (ASSUME COINS ARE FLOATING)
    FIBITMAP* fi_coin = FreeImage_Load(FIF_PNG, "assets/testCoin.png");
    staticObjectList[0] = staticObject(80, 80, fi_coin);
    staticObjectList[0].isCoin(1);
    staticObjectList[0].setPosition(300, 100);

    staticObjectList[0].setYOld(staticObjectList[0].getY());
    staticObjectList[0].setXOld(staticObjectList[0].getX());

    
    printf("COIN X: %d, COIN Y: %d, COIN W: %d, COIN H: %d", staticObjectList[0].getX(), staticObjectList[0].getY(), staticObjectList[0].getWidth(), staticObjectList[0].getHeight());


    //BALL GRAVITY
    int gravity = 1;



    //DATA TO BE PASSED TO CALLBACK
    callbackDataHolder callbackData;
    callbackData.ball_sprite = &ball_sprite;
    callbackData.bg = &bg_img;
    callbackData.maskObject = &maskObject;




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


     //INITIALIZE KEYBOARD INTERRUPTS
    mfb_set_keyboard_callback(window, key_press);
    mfb_set_user_data(window, (void*)&callbackData);



    do {

        ball_sprite.setY(ball_sprite.getY() + gravity);
        //if there is collision reverse this move
        if (ball_sprite.getY()>= 0 && !ball_sprite.detectCollision(maskObject, bg_img)) {
            
            if (bg_img.bg_y + gravity <= bg_img.height - window_height && bg_img.bg_y + window_height <= bg_img.height) {
                bg_img.bg_y += gravity;
            }
        }
        else {
            ball_sprite.setY(ball_sprite.getY() - gravity);
        }


        drawEntity(buffer, ball_sprite, bg_img, maskObject);
        drawEntity(buffer, staticObjectList[0], bg_img, maskObject);

        ball_sprite.setYOld(ball_sprite.getY());
        ball_sprite.setXOld(ball_sprite.getX());

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
    callbackDataHolder* callbackData= (callbackDataHolder*)mfb_get_user_data(window);
    Ball* ball_sprite = callbackData->ball_sprite;
    backgroundImageHolder* bg = callbackData->bg;
    staticObject* maskObject = callbackData->maskObject;


    if (isPressed) {

        //TODO: add conditions to detect collsions and prevent ball from going beyond the window borders

        if (key == KB_KEY_LEFT) {

            if (bg->bg_x -10 >= 0 && bg->bg_x <= bg->width - window_width) {
                bg->bg_x -= 10;

                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    bg->bg_x += 10;
                }

            }else if (ball_sprite->getX() - 10 >= 0) {
                ball_sprite->testMoveX(-10);
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    ball_sprite->testMoveX(10);
                }

            }
            
        }
        else if (key == KB_KEY_RIGHT) {

            if (bg->bg_x + 10 <= bg->width - window_width && bg->bg_x + window_width <= bg->width) {
                bg->bg_x += 10;
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    bg->bg_x -= 10;
                }
            }else if (ball_sprite->getX() + 10 <= window_width - ball_sprite->getWidth()) {
                ball_sprite->testMoveX(10);
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    ball_sprite->testMoveX(-10);
                }
            }


        }
        else if (key == KB_KEY_UP) {


            if (bg->bg_y - 10 >= 0 && bg->bg_y <= bg->height - window_height) {
                bg->bg_y -= 10;
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    bg->bg_y += 10;
                }
            }
            else if (ball_sprite->getY() - 10 >= 0) {
                ball_sprite->testMoveY(-10);
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    ball_sprite->testMoveY(10);
                }
            }

        }
        else if (key == KB_KEY_DOWN) {

            if (bg->bg_y + 10 <= bg->height - window_height && bg->bg_y + window_height <= bg->height) {
                bg->bg_y += 10;
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    bg->bg_y -= 10;
                }
            }
            else if (ball_sprite->getY() + 10 <= window_height - ball_sprite->getHeight()) {
                ball_sprite->testMoveY(10);
                if (ball_sprite->detectCollision(*maskObject, *bg)) {
                    ball_sprite->testMoveY(-10);
                }
            }



        }
        //else if (key == KB_KEY_W) {
        //    if (bg->bg_y - 10 >= 0 && bg->bg_y <= bg->height - window_height) {
        //        bg->bg_y -= 10;
        //    }
        //}
        //else if (key == KB_KEY_A) {
        //    if (bg->bg_y + 10 <= bg->height - window_height && bg->bg_y + window_height <= bg->height) {
        //        bg->bg_y += 10;
        //    }
        //}else if (key == KB_KEY_S) {
        //    if (bg->bg_x - 10 >= 0 && bg->bg_x <= bg->width - window_width) {
        //        bg->bg_x -= 10;

        //    }
        //}
        //else if (key == KB_KEY_D) {
        //    if (bg->bg_x + 10 <= bg->width - window_width && bg->bg_x + window_width <= bg->width) {
        //        bg->bg_x += 10;
        //    }
        //}

        printf("ball.isCollision value: %c\n", ball_sprite->detectCollision(*maskObject, *bg));



    }
}



