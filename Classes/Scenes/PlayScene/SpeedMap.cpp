#include "SpeedMap.h"
#include "../../Libs/GameLibrary.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

SpeedMap::SpeedMap(){

}
SpeedMap::~SpeedMap(){

}

bool SpeedMap::init(){

    if (!Layer::init()){
        return false;
    }

    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    int characterColor = 0;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        characterColor = dataManager->getCharacterColor();
        dataManager->close();
    }

    delete dataManager;

    Sprite* background = Sprite::createWithSpriteFrameName(Constants::BACKROUND_PATH);
    background->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    this->addChild(background);

    scoreLabel = Label::createWithTTF("0", Constants::DEFAULT_FONT_PATH, Constants::SCORE_FONT_SIZE);
    scoreLabel->setPosition(Vec2(origin.x + size.width / 2, origin.y + size.height * 0.8f));
    scoreLabel->setColor(Color3B(88, 105, 132));
    this->addChild(scoreLabel, 10);

    character = new Character(this, size, origin, characterColor, Constants::SPEED_MAP_ID);

    bubble = new Bubble(this, size, origin, true, Constants::SPEED_MAP_ID);

    bulletPackage = new BulletPackage(this, size, origin);

    infBulletPackage = new InfBulletPackage(this, size, origin);

    gameOver = false;
    score = 0;
    timer = 0.0f;

    bubbleContactToCharacterBullet = false;
    characterContactToPackage = false;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(SpeedMap::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}

void SpeedMap::update(float delta){
    if (!gameOver){
        timer += delta;

        int direction = controller->getDirection();

        if (direction != character->getDirection())
            character->changeDirection(direction);

        //Shooting
        if (controller->getShooting()){
            if (character->getNumRemainBullets() > 0){
                character->shoot();
                //Update bullets's remain
                controller->setBulletBarNum(character->getNumRemainBullets());
                //Play soundtrack
                if (SoundManager::isPlayingSoundtrack())
                    SoundManager::playSoundtrack(SoundManager::SHOOTING_AUDIO);
            }
            controller->setShooting(false);
        }

        //Reset bullet
        if (((int)timer) % Constants::CHARACTER_BULLET_RESET_TIME == 0){
            if (character->isResetingBullet()){
                if (character->getNumRemainBullets() < Constants::CHARACTER_BULLET_MAXIMUM){
                    character->increaseNumRemainBullets();
                    //Update bullets's remain
                    controller->setBulletBarNum(character->getNumRemainBullets());
                }
                character->setResetingBullet(false);
            }
        }
        else {
            character->setResetingBullet(true);
        }

        character->update();

        bubble->update(timer);

        bulletPackage->update();

        infBulletPackage->update(delta);

        //Auto reset bullet
        if (infBulletPackage->isActive()){
            character->resetBulletSet();
            //Update bullets's remain
            controller->setBulletBarNum(character->getNumRemainBullets());
            //Out of inf shooting time
            if (infBulletPackage->getActiveTime() >= Constants::INF_BULLET_PACKAGE_ACTIVE_TIME){
                controller->changeBulletBarColor(Color3B::RED);
                infBulletPackage->setActive(false);
            }
        }
        else {}

        ///////////////////PHYSICS/////////////////////////////////

        if (bubbleContactToCharacterBullet){
            PhysicsBody* bodyBubble = (bodyA->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK)
                                        ? bodyA : bodyB;
            PhysicsBody* bodyBullet = (bodyA->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK)
                                        ? bodyB : bodyA;
            //Update score's number to label, based in size of bubble
            score += (Constants::BUBBLE_BIG_TAG - bodyBubble->getNode()->getTag() + 1) * 3;
            scoreLabel->setString(std::to_string(score));
            //Delete bullet
            auto bulletShooted = bodyBullet->getNode();
            for (unsigned int i = 0; i < character->getBulletSet()->size(); ++i){
                auto bulletMatched = character->getBulletSet()->at(i);
                if (bulletShooted == bulletMatched){
                    character->getBulletSet()->erase(character->getBulletSet()->begin() + i);
                    bulletShooted->removeFromParent();
                    break;
                }
            }
            //Make bubble smaller or disappear
            auto bubbleShooted = bodyBubble->getNode();
            int bubbleSize = bodyBubble->getNode()->getTag();
            for (unsigned int i = 0; i < bubble->getBubbleSet()->size(); ++i){
                auto bubbleMatched = bubble->getBubbleSet()->at(i);
                if (bubbleShooted == bubbleMatched){
                    auto bubblePosition = bubbleShooted->getPosition();
                    bubble->getBubbleSet()->erase(bubble->getBubbleSet()->begin() + i);
                    bubbleShooted->removeFromParent();
                    if (bubbleSize != Constants::BUBBLE_SMALL_TAG){
                        bubble->startAddTwoBubble(bubblePosition, bubbleSize-1);
                    }
                    //Play soundtrack
                    if (SoundManager::isPlayingSoundtrack())
                        SoundManager::playSoundtrack(SoundManager::EXPLOSION_AUDIO);
                    break;
                }
            }
            bubbleContactToCharacterBullet = false;
        }

        if (characterContactToPackage){
            PhysicsBody* bodyPackage = (bodyA->getCategoryBitmask() == Constants::PACKAGE_CATEGORY_BITMASK)
                                        ? bodyA : bodyB;
            //Play soundtrack
            if (SoundManager::isPlayingSoundtrack())
                SoundManager::playSoundtrack(SoundManager::ITEM_PICKUP_AUDIO);

            if (bodyPackage->getNode()->getTag() == Constants::BULLET_PACKAGE_ID){
                character->resetBulletSet();
                //Update bullets's remain
                controller->setBulletBarNum(character->getNumRemainBullets());
                //Make package disappear and change it's direction
                bodyPackage->getNode()->setPosition(origin.x + size.width * 1.25f,
                                                    origin.y + size.height * 0.5f);
                bodyPackage->getNode()->stopAllActions();
                Vec2 vecMovement = Vec2(RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V),
                                        RandomHelper::random_real(-Constants::PACKAGE_V, Constants::PACKAGE_V));
                bodyPackage->getNode()->getPhysicsBody()->setVelocity(vecMovement);
            }
            else if (bodyPackage->getNode()->getTag() == Constants::INF_BULLET_PACKAGE_ID){
                bodyPackage->getNode()->setPosition(origin.x + size.width * 1.25f,
                                                    origin.y + size.height * 0.5f);
                infBulletPackage->setActive(true);
                controller->changeBulletBarColor(Color3B::GREEN);
            }
            else {}
            characterContactToPackage = false;
        }
    }
    else {
        //Game over

        //Play soundtrack
        if (SoundManager::isPlayingSoundtrack())
            SoundManager::playSoundtrack(SoundManager::DIE_AUDIO);

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
        character->stop();

        this->unscheduleUpdate();

        scoreLabel->removeFromParent();
        controller->removeFromParent();

        endLayer = EndLayer::create();
        endLayer->setLabel(score);
        this->addChild(endLayer, 10);
    }
}

void SpeedMap::close(Ref* sender){
    delete character;
    delete bubble;
    delete bulletPackage;
    delete infBulletPackage;

    this->removeFromParentAndCleanup(true);
}

void SpeedMap::setController(ControlLayer* controller){
    this->controller = controller;
}

bool SpeedMap::onContactBegin(cocos2d::PhysicsContact &contact){
    PhysicsBody* bodyA = contact.getShapeA()->getBody();
    PhysicsBody* bodyB = contact.getShapeB()->getBody();
    if ((bodyA->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK) ||
        (bodyB->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK &&
        bodyA->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK)){
            //Game over
            gameOver = true;
    }
    else if ((bodyA->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::CHARACTER_BULLET_CATEGORY_BITMASK) ||
        (bodyA->getCategoryBitmask() == Constants::CHARACTER_BULLET_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK)){
            this->bodyA = bodyA;
            this->bodyB = bodyB;
            bubbleContactToCharacterBullet = true;
    }
    else if ((bodyA->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::PACKAGE_CATEGORY_BITMASK) ||
        (bodyA->getCategoryBitmask() == Constants::PACKAGE_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK)){
            this->bodyA = bodyA;
            this->bodyB = bodyB;
            characterContactToPackage = true;
    }
    else {
    }
    return true;
}
