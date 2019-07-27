#ifndef __BOSS_MAP_H__
#define __BOSS_MAP_H__

#include "cocos2d.h"
#include "ControlLayer.h"
#include "Character.h"
#include "Boss.h"
#include "BulletPackage.h"
#include "InfBulletPackage.h"
#include "EndLayer.h"
#include "Boss.h"
#include <vector>

USING_NS_CC;

class BossMap : public Layer

{
public:
    BossMap();
    ~BossMap();

    bool init() override;
    void update(float) override;

    CREATE_FUNC(BossMap);

    void close(Ref* sender=nullptr);

    void setController(ControlLayer* controller);

private:
    Size size;
    Vec2 origin;

    Character* character;
    BulletPackage* bulletPackage;
    InfBulletPackage* infBulletPackage;

    std::vector<Boss*> *bosses;

    float timer;
    int score;
    bool gameOver;

    bool characterBulletContactToBoss;
    bool characterContactToPackage;

    bool isPlayingSoundtrack;
    bool isPlayingBackgroundMusic;

    EndLayer* endLayer;

    Label* scoreLabel;

    PhysicsBody* bodyA;
    PhysicsBody* bodyB;

    ControlLayer* controller;

    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif
