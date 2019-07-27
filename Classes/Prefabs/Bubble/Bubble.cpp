#include "Bubble.h"
#include "../../Libs/GameLibrary.h"

USING_NS_CC;

Bubble::Bubble(Node* parentLayer, Size size, Vec2 origin, bool withPhysicBody, int worldID){
    this->parentLayer = parentLayer;
    this->size = size;
    this->origin = origin;
    this->withPhysicBody = withPhysicBody;

    float bubbleInitRatio = 1.0f;
    speedIncrease = 0;
    speedRatio = 0.0f;
    scaleRatio = 1.0f;

    switch (worldID){
        case Constants::MENU_MAP_ID:{
            break;
        }
        case Constants::NORMAL_MAP_ID:
        {
            break;
        }
        case Constants::SPEED_MAP_ID:
        {
            bubbleInitRatio = 0.9f;
            speedRatio = 1.0f;
            scaleRatio = 0.66f;
            speedIncrease = 2;
            break;
        }
        case Constants::WINDMILL_MAP_ID:
        {
            bubbleInitRatio = 1.0f;
            speedRatio = 0.5f;
            scaleRatio = 0.75f;
            speedIncrease = 1;
            break;
        }
        case Constants::BOSS_MAP_ID:
        {
            break;
        }
        default:
        break;
    }

    bubbleSet = new std::vector<Sprite*>();
    increaseBubble = false;

    addTwoBubble = false;
    twoBubbleAddedPosition = Vec2(0,0);
    twoBubbleAddedSize = 0;

    for (int i = 0; i < (int)(Constants::BUBBLE_INIT_NUM * bubbleInitRatio); ++i)
        addNewBubble();
}

Bubble::~Bubble(){
    delete bubbleSet;
}

void Bubble::addNewBubble(){
    int bubbleSize = RandomHelper::random_int(0, 2);

    Vec2 bubblePosition;
    if (RandomHelper::random_int(0,1)){
        //Init in right outside
        bubblePosition = Vec2(origin.x + size.width * 1.25f, origin.y + size.height * 0.5f);
    }
    else {
        //Init in left outside
        bubblePosition = Vec2(origin.x - size.width * 0.25f, origin.y + size.height * 0.5f);
    }

    addNewBubble(bubblePosition, bubbleSize);
}

void Bubble::addNewBubble(Vec2 position, int bubbleSize){

    int bubbleColor = RandomHelper::random_int(1, Constants::BUBBLE_NUM_COLOR);

    Sprite* bubble = Sprite::createWithSpriteFrameName(Constants::BUBBLE_PATH + std::to_string(bubbleColor) + ".png");
    bubble->setPosition(origin.x + position.x, origin.y + position.y);
    bubble->setTag(bubbleSize);
    bubble->setScale(pow(2, bubbleSize - Constants::BUBBLE_BIG_TAG) * scaleRatio);

    Vec2 vecMovement = Vec2(RandomHelper::random_real(-Constants::BUBBLE_V, Constants::BUBBLE_V),
                            RandomHelper::random_real(-Constants::BUBBLE_V, Constants::BUBBLE_V));
    // Set speed
    vecMovement.x += ((vecMovement.x < 0) ? (-Constants::BUBBLE_V * speedRatio) : (Constants::BUBBLE_V * speedRatio));
    vecMovement.y += ((vecMovement.y < 0) ? (-Constants::BUBBLE_V * speedRatio) : (Constants::BUBBLE_V * speedRatio));

    if (withPhysicBody){
        auto physicsBody = PhysicsBody::createCircle(bubble->getContentSize().width / 2);

        physicsBody->setCategoryBitmask(Constants::BUBBLE_CATEGORY_BITMASK);
        physicsBody->setCollisionBitmask(Constants::BUBBLE_COLLISION_BITMASK);
        physicsBody->setContactTestBitmask(Constants::BUBBLE_COLLISION_BITMASK);

        physicsBody->setVelocity(vecMovement);
        physicsBody->applyImpulse(vecMovement * 20);

        bubble->setPhysicsBody(physicsBody);
    }

    else {
        auto move = MoveBy::create(1.0f, vecMovement);
        auto actionMove = RepeatForever::create(move);
        bubble->runAction(actionMove);
    }

    bubbleSet->push_back(bubble);
    parentLayer->addChild(bubble, Constants::BUBBLE_Z_ORDER);
}

std::vector<Sprite*>* Bubble::getBubbleSet(){
    return this->bubbleSet;
}

void Bubble::startAddTwoBubble(Vec2 position, int bubbleSize){
    twoBubbleAddedPosition = position;
    twoBubbleAddedSize = bubbleSize;
    addTwoBubble = true;
}

void Bubble::update(float timer){
    //Make bubbles go by the wall
    for (unsigned int i = 0; i < bubbleSet->size(); ++i){
        float x = bubbleSet->at(i)->getPosition().x;
        float y = bubbleSet->at(i)->getPosition().y;

        if (x < origin.x - Constants::BUBBLE_OUT_SPACE){
            x = origin.x + size.width + Constants::BUBBLE_OUT_SPACE;
            bubbleSet->at(i)->setPosition(x,y);
        }

        if (x > origin.x + size.width + Constants::BUBBLE_OUT_SPACE){
            x = origin.x - Constants::BUBBLE_OUT_SPACE;
            bubbleSet->at(i)->setPosition(x,y);
        }

        if (y < origin.y - Constants::BUBBLE_OUT_SPACE){
            y = origin.y + size.height + Constants::BUBBLE_OUT_SPACE;
            bubbleSet->at(i)->setPosition(x,y);
        }

        if (y > origin.y + size.height + Constants::BUBBLE_OUT_SPACE){
            y = origin.y - Constants::BUBBLE_OUT_SPACE;
            bubbleSet->at(i)->setPosition(x,y);
        }
    }

    //Add 2 new bubbles when physic occurs
    if (addTwoBubble){
        addNewBubble(twoBubbleAddedPosition, twoBubbleAddedSize);
        addNewBubble(twoBubbleAddedPosition, twoBubbleAddedSize);
        addTwoBubble = false;
    }

    //Increase bubble over time
    if (((int)timer) % (Constants::BUBBLE_INCREASE_TIME - speedIncrease) == 0){
        if (increaseBubble && bubbleSet->size() <= Constants::BUBBLE_MAX_NUM){
            addNewBubble();
            increaseBubble = false;
        }
    }
    else {
        increaseBubble = true;
    }
}
