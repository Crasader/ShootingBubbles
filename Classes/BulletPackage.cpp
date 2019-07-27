#include "BulletPackage.h"
#include "GameLibrary.h"

BulletPackage::BulletPackage(Node* playLayer, Size size, Vec2 origin){
    this->playLayer = playLayer;
    this->size = size;
    this->origin = origin;

    for (int i = 0; i < Constants::BULLET_PACKAGE_NUM; ++i) {
        Sprite* package = Sprite::createWithSpriteFrameName(Constants::BULLET_PACKAGE_PATH);
        package->setTag(Constants::BULLET_PACKAGE_ID);

        auto physicsBody = PhysicsBody::createBox(package->getContentSize());

        physicsBody->setCategoryBitmask(Constants::PACKAGE_CATEGORY_BITMASK);
        physicsBody->setCollisionBitmask(Constants::PACKAGE_COLLISION_BITMASK);
        physicsBody->setContactTestBitmask(Constants::PACKAGE_COLLISION_BITMASK);
        package->setPhysicsBody(physicsBody);

        package->setPosition(origin.x + size.width * 1.25f,
                            origin.y + size.height * 0.5f);

        Vec2 vecMovement = Vec2(RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V),
                                RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V));
        physicsBody->setVelocity(vecMovement);

        bulletPackages[i] = package;
        playLayer->addChild(package, Constants::PACKAGE_Z_ORDER);
    }
}

BulletPackage::~BulletPackage(){
}

Sprite** BulletPackage::getBulletPackages(){
    return this->bulletPackages;
}

void BulletPackage::update(){
    for (int i = 0; i < Constants::BULLET_PACKAGE_NUM; ++i){
        float x = bulletPackages[i]->getPosition().x;
        float y = bulletPackages[i]->getPosition().y;

        if (x < origin.x - Constants::BUBBLE_OUT_SPACE){
            x = origin.x + size.width + Constants::BUBBLE_OUT_SPACE;
            bulletPackages[i]->setPosition(x,y);
        }

        if (x > origin.x + size.width + Constants::BUBBLE_OUT_SPACE){
            x = origin.x - Constants::BUBBLE_OUT_SPACE;
            bulletPackages[i]->setPosition(x,y);
        }

        if (y < origin.y - Constants::BUBBLE_OUT_SPACE){
            y = origin.y + size.height + Constants::BUBBLE_OUT_SPACE;
            bulletPackages[i]->setPosition(x,y);
        }

        if (y > origin.y + size.height + Constants::BUBBLE_OUT_SPACE){
            y = origin.y - Constants::BUBBLE_OUT_SPACE;
            bulletPackages[i]->setPosition(x,y);
        }
    }
}
