#include "Character.h"
#include "../../Libs/GameLibrary.h"

USING_NS_CC;

Character::Character(Node* playLayer, Size size, Vec2 origin, int characterColor, int worldID){
    this->playLayer = playLayer;
    this->size = size;
    this->origin = origin;
    this->direction = 0;

    characterMovementSpeedRatio = 1.0f;
    bulletMovementSpeedRatio = 1.0f;

    float characterScale = 1.0f;

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
            characterMovementSpeedRatio = 1.5f;
            bulletMovementSpeedRatio = 1.5f;
            characterScale = 0.75f;
            break;
        }
        case Constants::WINDMILL_MAP_ID:
        {
            characterScale = 0.75f;
            break;
        }
        case Constants::BOSS_MAP_ID:
        {
            bulletMovementSpeedRatio = 1.25f;
            characterScale = 0.75f;
            break;
        }
        default:
        break;
    }

    bulletSet = new std::vector<Sprite*>();
    numRemainBullets = Constants::CHARACTER_BULLET_MAXIMUM;
    resetBullet = false;

    std::string spritePath = Constants::CHARACTER_PATH + std::to_string(characterColor) + ".png";

    characterSprite = Sprite::createWithSpriteFrameName(spritePath);
    characterSprite->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);

    characterSprite->setScale(characterScale);

    float characterSize = characterSprite->getContentSize().width;
    Vec2 physicsBodyPoints[] = {
        Vec2(0, characterSize * 0.5f),
        Vec2(characterSize * 0.5f, -characterSize * 0.5),
        Vec2(-characterSize * 0.5f, -characterSize * 0.5)
    };

    auto physicsBody = PhysicsBody::createPolygon(physicsBodyPoints, 3);

    physicsBody->setContactTestBitmask(Constants::CHARACTER_COLLISION_BITMASK);
    physicsBody->setCollisionBitmask(Constants::CHARACTER_COLLISION_BITMASK);
    physicsBody->setCategoryBitmask(Constants::CHARACTER_CATEGORY_BITMASK);
    characterSprite->setPhysicsBody(physicsBody);

    playLayer->addChild(characterSprite, Constants::CHARACTER_Z_ORDER);

    auto rotate = RotateBy::create(Constants::CHARACTER_ROTATE_SPEED, 360.0f);
    auto action = RepeatForever::create(rotate);
    characterSprite->runAction(action);
}

Character::~Character(){
    delete bulletSet;
}

void Character::stop(){
    characterSprite->stopAllActions();
}

void Character::changeDirection(int direction){
    Vec2 vecDiff;
    this->direction = direction;
    switch (direction) {
        case Constants::DIR_LEFT:
            vecDiff = Vec2(-Constants::CHARACTER_V, 0);
            break;
        case Constants::DIR_RIGHT:
            vecDiff = Vec2(Constants::CHARACTER_V, 0);
            break;
        case Constants::DIR_UP:
            vecDiff = Vec2(0, Constants::CHARACTER_V);
            break;
        case Constants::DIR_DOWN:
            vecDiff = Vec2(0, -Constants::CHARACTER_V);
            break;
        default:
            vecDiff = Vec2(0, 0);
            break;
    }

    // Set speed

    vecDiff.x *= characterMovementSpeedRatio;
    vecDiff.y *= characterMovementSpeedRatio;

    characterSprite->getPhysicsBody()->setVelocity(vecDiff);
}

int Character::getDirection(){
    return this->direction;
}

void Character::shoot(){

    float rotation = characterSprite->getRotation();
    Vec2 center = characterSprite->getPosition();
    float ratio = characterSprite->getContentSize().width / 2;

    float headX = center.x + ratio * sin(rotation * Constants::PI / 180);
    float headY = center.y + ratio * cos(rotation * Constants::PI / 180);

    numRemainBullets--;

    float deltaX = (headX - center.x) * Constants::CHARACTER_BULLET_V * bulletMovementSpeedRatio;
    float deltaY = (headY - center.y) * Constants::CHARACTER_BULLET_V * bulletMovementSpeedRatio;
    Sprite* bullet = Sprite::createWithSpriteFrameName(Constants::CHARACTER_BULLET_PATH);
    bullet->setPosition(Vec2(headX, headY));

    bullet->setScale(characterSprite->getScale());

    auto physicsBody = PhysicsBody::createCircle(bullet->getContentSize().width / 2);
    physicsBody->setContactTestBitmask(Constants::CHARACTER_BULLET_COLLISION_BITMASK);
    physicsBody->setCollisionBitmask(Constants::CHARACTER_BULLET_COLLISION_BITMASK);
    physicsBody->setCategoryBitmask(Constants::CHARACTER_BULLET_CATEGORY_BITMASK);

    physicsBody->setVelocity(Vec2(deltaX, deltaY));

    bullet->setPhysicsBody(physicsBody);

    this->playLayer->addChild(bullet, Constants::CHARACTER_BULLET_Z_ORDER);
    bulletSet->push_back(bullet);
}

std::vector<Sprite*>* Character::getBulletSet(){
    return this->bulletSet;
}

int Character::getNumRemainBullets(){
    return numRemainBullets;
}

void Character::increaseNumRemainBullets(){
    ++numRemainBullets;
}

bool Character::isResetingBullet(){
    return resetBullet;
}
void Character::setResetingBullet(bool value){
    resetBullet = value;
}

void Character::resetBulletSet(){
    while (numRemainBullets < Constants::CHARACTER_BULLET_MAXIMUM)
        ++numRemainBullets;
}

void Character::update(){

    Point position = characterSprite->getPosition();

    //Make character goes by the walls
    if (position.x < origin.x - 2 * Constants::OUT_SPACE) {
        position.x = size.width + origin.x + Constants::OUT_SPACE;
        characterSprite->setPosition(position);
    }

    if (position.y < origin.y - 2 * Constants::OUT_SPACE) {
        position.y = size.height + origin.y + Constants::OUT_SPACE;
        characterSprite->setPosition(position);
    }

    if (position.x > origin.x + size.width + 2 * Constants::OUT_SPACE) {
        position.x = origin.x - Constants::OUT_SPACE;
        characterSprite->setPosition(position);
    }

    if (position.y > origin.y + size.height + 2 * Constants::OUT_SPACE){
        position.y = origin.y - Constants::OUT_SPACE;
        characterSprite->setPosition(position);
    }

    //Delete bullet if it's not inside screen
    for (unsigned int i = 0; i < bulletSet->size(); ++i){
        float x = bulletSet->at(i)->getPosition().x;
        float y = bulletSet->at(i)->getPosition().y;
        if (x < origin.x - Constants::OUT_SPACE || y < origin.y - Constants::OUT_SPACE ||
            x > origin.x + size.width + Constants::OUT_SPACE || y > origin.y + size.height + Constants::OUT_SPACE){
                auto node = bulletSet->at(i);
                bulletSet->erase(bulletSet->begin() + i);
                node->removeFromParent();
        }
    }
}
