#ifndef __BULLET_PACKAGE_H__
#define __BULLET_PACKAGE_H__

#include "cocos2d.h"

USING_NS_CC;

class BulletPackage

{
public:
    BulletPackage(Node*, Size, Vec2);
    ~BulletPackage();

    Sprite** getBulletPackages();

    void update();

private:
    Node* playLayer;
    Size size;
    Vec2 origin;

    Sprite* bulletPackages[Constants::BULLET_PACKAGE_NUM];
};

#endif
