#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "cocos2d.h"
#include <cmath>

USING_NS_CC;

class Character

{
public:
    Character(Node*, Size, Vec2, int characterColor, int worldID);
    ~Character();

    void changeDirection(int direction);
    int getDirection();

    void shoot();
    std::vector<Sprite*>* getBulletSet();
    int getNumRemainBullets();
    void increaseNumRemainBullets();
    bool isResetingBullet();
    void setResetingBullet(bool);
    void resetBulletSet();

    void stop();
    void update();

private:
    Node* playLayer;
    Size size;
    Vec2 origin;

    int direction;
    Sprite* characterSprite;

    float characterMovementSpeedRatio;
    float bulletMovementSpeedRatio;

    std::vector<Sprite*> *bulletSet;
    int numRemainBullets;
    bool resetBullet;
};

#endif
