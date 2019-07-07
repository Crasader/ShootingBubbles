#ifndef __BUBBLE_H__
#define __BUBBLE_H__

#include "cocos2d.h"
#include "Constants.h"
#include <cmath>
#include <vector>

USING_NS_CC;

class Bubble

{
public:
    Bubble(Node* parentLayer, Size size, Vec2 origin, bool withPhysicBody, int worldID);
    ~Bubble();

    void init();

    void startAddTwoBubble(Vec2 position, int bubbleSize);

    void update(float timer);

    std::vector<Sprite*>* getBubbleSet();

private:
    Node* parentLayer;
    Size size;
    Vec2 origin;

    bool withPhysicBody;

    std::vector<Sprite*> *bubbleSet;
    bool increaseBubble;

    bool addTwoBubble;
    Vec2 twoBubbleAddedPosition;
    int twoBubbleAddedSize;

    float speedRatio;
    int speedIncrease;
    float scaleRatio;

    void addNewBubble();
    void addNewBubble(Vec2 position, int bubbleSize);
};

#endif
