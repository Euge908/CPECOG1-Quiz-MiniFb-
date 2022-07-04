#pragma once
#include "Entity.h"


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
