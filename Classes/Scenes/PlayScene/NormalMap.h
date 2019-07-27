#ifndef __NORMAL_MAP_H__
#define __NORMAL_MAP_H__

#include "cocos2d.h"
#include "../../Libs/ControlLayer.h"
#include "../../Prefabs/Character/Character.h"
#include "../../Prefabs/Boss/Boss.h"
#include "../../Prefabs/BulletPackage/BulletPackage.h"
#include "../../Prefabs/BulletPackage/InfBulletPackage.h"
#include "../../Prefabs/Bubble/Bubble.h"
#include "EndLayer.h"
#include <vector>

USING_NS_CC;

class NormalMap : public Layer

{
public:
    NormalMap();
    ~NormalMap();

    bool init() override;
    void update(float) override;

    CREATE_FUNC(NormalMap);

    void close(Ref* sender=nullptr);

    void setController(ControlLayer* controller);

private:
    Size size;
    Vec2 origin;

    ControlLayer* controller;
    Character* character;
    Bubble* bubble;
    BulletPackage* bulletPackage;
    InfBulletPackage* infBulletPackage;

    std::vector<Boss*> *bosses;

    Label* scoreLabel;
    float timer;

    int score;
    bool gameOver;

    int bossLevel;

    EndLayer* endLayer;

    bool onContactBegin(cocos2d::PhysicsContact &contact);

    // Contact attributes
    bool bubbleContactToCharacterBullet;
    bool characterContactToPackage;
    bool characterBulletContactToBoss;

    PhysicsBody* bodyA;
    PhysicsBody* bodyB;
};

#endif
