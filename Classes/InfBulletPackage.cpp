#include "InfBulletPackage.h"


InfBulletPackage::InfBulletPackage(Node* playLayer, Size size, Vec2 origin){
    this->playLayer = playLayer;
    this->size = size;
    this->origin = origin;

    infBulletPackage = Sprite::createWithSpriteFrameName(Constants::INF_BULLET_PACKAGE_PATH);
    infBulletPackage->setTag(Constants::INF_BULLET_PACKAGE_ID);

    auto physicsBody = PhysicsBody::createBox(infBulletPackage->getContentSize());

    physicsBody->setCategoryBitmask(Constants::PACKAGE_CATEGORY_BITMASK);
    physicsBody->setCollisionBitmask(Constants::PACKAGE_COLLISION_BITMASK);
    physicsBody->setContactTestBitmask(Constants::PACKAGE_COLLISION_BITMASK);
    infBulletPackage->setPhysicsBody(physicsBody);

    infBulletPackage->setPosition(origin.x + size.width * 1.25f,
                        origin.y + size.height * 0.5f);

    Vec2 vecMovement = Vec2(RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V),
                            RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V));
    physicsBody->setVelocity(vecMovement);

    playLayer->addChild(infBulletPackage, Constants::PACKAGE_Z_ORDER);

    inScreenTime = 0.0f;
    outScreenTime = 0.0f;
    activeTime = 0.0f;

    inScreen = false;
    active = false;

}

InfBulletPackage::~InfBulletPackage(){

}

void InfBulletPackage::changeDirection(){
    Vec2 vecMovement = Vec2(RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V),
                            RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V));
    infBulletPackage->getPhysicsBody()->setVelocity(vecMovement);
}

void InfBulletPackage::update(float delta){
    if (active){
        activeTime += delta;
    }
    else {
        if (inScreen){
            inScreenTime += delta;
            //Make inf bullet package not go out
            float x = infBulletPackage->getPosition().x;
            float y = infBulletPackage->getPosition().y;

            if (x < origin.x - Constants::BUBBLE_OUT_SPACE){
                x = origin.x + size.width + Constants::BUBBLE_OUT_SPACE;
                infBulletPackage->setPosition(x,y);
            }

            if (x > origin.x + size.width + Constants::BUBBLE_OUT_SPACE){
                x = origin.x - Constants::BUBBLE_OUT_SPACE;
                infBulletPackage->setPosition(x,y);
            }

            if (y < origin.y - Constants::BUBBLE_OUT_SPACE){
                y = origin.y + size.height + Constants::BUBBLE_OUT_SPACE;
                infBulletPackage->setPosition(x,y);
            }

            if (y > origin.y + size.height + Constants::BUBBLE_OUT_SPACE){
                y = origin.y - Constants::BUBBLE_OUT_SPACE;
                infBulletPackage->setPosition(x,y);
            }
        }
        else {
            outScreenTime += delta;
            if (outScreenTime >= Constants::INF_BULLET_PACKAGE_OUT_SCREEN_TIME){
                //Make inf bullet package in screen
                inScreen = true;
                inScreenTime = 0.0f;
                outScreenTime = 0.0f;
                changeDirection();
            }
        }
    }
}

void InfBulletPackage::setActive(bool value){
    active = value;
    if (!active) activeTime = 0.0f;
    else {
        inScreenTime = 0.0f;
        outScreenTime = 0.0f;
        inScreen = false;
    }
}

bool InfBulletPackage::isActive(){
    return active;
}

float InfBulletPackage::getActiveTime(){
    return activeTime;
}
