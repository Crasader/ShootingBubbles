#ifndef __INF_BULLET_PACKAGE_H__
#define __INF_BULLET_PACKAGE_H__

#include "cocos2d.h"

USING_NS_CC;

class InfBulletPackage

{
public:
    InfBulletPackage(Node*, Size, Vec2);
    ~InfBulletPackage();

    void update(float delta);

    void setActive(bool);
    bool isActive();
    float getActiveTime();

private:
    Node* playLayer;
    Size size;
    Vec2 origin;

    Sprite* infBulletPackage;

    bool inScreen;
    float inScreenTime;
    float outScreenTime;

    float activeTime;
    bool active;

    void changeDirection();
};

#endif
