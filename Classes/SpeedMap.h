#ifndef __SPEED_MAP_H__
#define __SPEED_MAP_H__

#include "cocos2d.h"
#include "ControlLayer.h"
#include "Bubble.h"
#include "Character.h"
#include "BulletPackage.h"
#include "InfBulletPackage.h"
#include "EndLayer.h"
#include <vector>

USING_NS_CC;

class SpeedMap : public Layer

{
public:
    SpeedMap();
    ~SpeedMap();

    bool init() override;
    void update(float) override;

    CREATE_FUNC(SpeedMap);

    void close(Ref* sender=nullptr);

    void setController(ControlLayer* controller);

private:

    Size size;
    Vec2 origin;

    Bubble* bubble;
    Character* character;
    BulletPackage* bulletPackage;
    InfBulletPackage* infBulletPackage;

    float timer;
    int score;
    bool gameOver;

    bool bubbleContactToCharacterBullet;
    bool characterContactToPackage;

    EndLayer* endLayer;

    Label* scoreLabel;

    PhysicsBody* bodyA;
    PhysicsBody* bodyB;

    ControlLayer* controller;

    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif
