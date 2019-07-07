#ifndef __WIND_MILL_MAP_H__
#define __WIND_MILL_MAP_H__

#include "cocos2d.h"
#include "ControlLayer.h"
#include "Constants.h"
#include "Bubble.h"
#include "Character.h"
#include "BulletPackage.h"
#include "InfBulletPackage.h"
#include "EndLayer.h"
#include <vector>

USING_NS_CC;

class WindMillMap : public Layer

{
public:
    WindMillMap();
    ~WindMillMap();

    bool init() override;
    void update(float) override;

    CREATE_FUNC(WindMillMap);

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

    bool isPlayingSoundtrack;
    bool isPlayingBackgroundMusic;

    bool bubbleContactToCharacterBullet;
    bool characterContactToPackage;

    EndLayer* endLayer;

    Label* scoreLabel;

    PhysicsBody* bodyA;
    PhysicsBody* bodyB;

    ControlLayer* controller;

    void addWindMills();
    bool onContactBegin(cocos2d::PhysicsContact &contact);
};

#endif
