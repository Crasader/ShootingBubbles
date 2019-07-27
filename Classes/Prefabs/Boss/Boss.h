#ifndef __BOSS_H__
#define __BOSS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <cmath>
#include <vector>

USING_NS_CC;

class Boss

{
public:
    Boss(Node*, Size, Vec2, bool isScale);
    ~Boss();

    Vec2 getPosition();
    void setPosition(Point pos);

    void changeDirectionVertically();
    void changeDirectionHorizontally();

    Sprite* getBossSprite();
    std::vector<Sprite*> *getBulletSet();

    void shoot();

    void setHP(int hp);
    int getHP();

    bool bulletIsEmpty();

    void update();

    void stop();

private:
    Node* playLayer;
    Size size;
    Vec2 origin;

    Node* container;
    Sprite* bossSprite;
    std::vector<Sprite*> *bossBulletSet;
    Vec2 *headPosition;

    Vec2 vecMovement;

    int HP, MAX_HP, NUM_OF_BULLET_HEAD;
    ui::LoadingBar *hpBar;
};

#endif
