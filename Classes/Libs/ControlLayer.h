#ifndef __CONTROLLER_LAYER_H__
#define __CONTROLLER_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class ControlLayer : public Layer

{
public:
    ControlLayer();
    ~ControlLayer();

    bool init() override;
    virtual bool onTouchBegan(cocos2d::Touch*, cocos2d::Event*) override;
    virtual void onTouchMoved(cocos2d::Touch*, cocos2d::Event*) override;
    virtual void onTouchEnded(cocos2d::Touch*, cocos2d::Event*) override;

    CREATE_FUNC(ControlLayer);

    void close(Ref* sender=nullptr);

    int getDirection();

    bool getShooting();
    void setShooting(bool value);

    void setBulletBarNum(int num);
    void changeBulletBarColor(Color3B color);

private:
    Size size;
    Vec2 origin;

    int directionOf(Vec2 point, Vec2 centerPoint);
    int direction;

    bool isShoot;
    ProgressTimer* bulletBar;
};

#endif
