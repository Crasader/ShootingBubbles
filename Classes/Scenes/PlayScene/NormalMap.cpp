#include "NormalMap.h"
#include "../../Libs/GameLibrary.h"

USING_NS_CC;

NormalMap::NormalMap(){

}
NormalMap::~NormalMap(){

}

bool NormalMap::init(){

    if (!Layer::init()){
        return false;
    }

    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    //Load data from data manager

    int characterColor = 0;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        characterColor = dataManager->getCharacterColor();
        dataManager->close();
    }

    delete dataManager;

    //Set up

    Sprite* background = Sprite::createWithSpriteFrameName(Constants::BACKROUND_PATH);
    background->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    this->addChild(background);

    character = new Character(this, size, origin, characterColor, Constants::NORMAL_MAP_ID);

    bubble = new Bubble(this, size, origin, true, Constants::NORMAL_MAP_ID);

    bulletPackage = new BulletPackage(this, size, origin);

    infBulletPackage = new InfBulletPackage(this, size, origin);

    bosses = new std::vector<Boss*>();

    scoreLabel = Label::createWithTTF("0", Constants::DEFAULT_FONT_PATH, Constants::SCORE_FONT_SIZE);
    scoreLabel->setPosition(Vec2(origin.x + size.width / 2, origin.y + size.height * 0.8f));
    scoreLabel->setColor(Color3B(88, 105, 132));
    this->addChild(scoreLabel, 10);

    timer = 0.0f;
    score = 0;

    bossLevel = 1;

    gameOver = false;

    bubbleContactToCharacterBullet = false;
    characterContactToPackage = false;
    characterBulletContactToBoss = false;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(NormalMap::onContactBegin, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();

    return true;
}

void NormalMap::update(float delta){

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
        if (((int)timer) % (Constants::CHARACTER_BULLET_RESET_TIME) == 0){
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

        //Boss ////////////////////////////////////////////////

        for (unsigned int i = 0; i < bosses->size(); ++i){
            //Make bosses not go out screen
            auto boss = bosses->at(i);
            boss->update();

            //Boss Shoot
            if (bosses->at(i)->bulletIsEmpty()){
                bosses->at(i)->shoot();
                //Play soundtrack
                if (SoundManager::isPlayingSoundtrack())
                    SoundManager::playSoundtrack(SoundManager::BOSS_SHOOTING_AUDIO);
            }

            //Check if boss's hp = 0
            if (boss->getHP() == 0){
                //Play soundtrack
                if (SoundManager::isPlayingSoundtrack())
                    SoundManager::playSoundtrack(SoundManager::DIE_AUDIO);

                boss->getBossSprite()->removeFromParent();
                bosses->erase(bosses->begin() + i);
                delete boss;
                break;
            }
        }

        //Push new boss after score up to range
        if (score >= bossLevel * Constants::BOSS_EXIST_BASE_SCORE){
            bosses->push_back(new Boss(this, size, origin, false));
            ++bossLevel;
        }

        ///////////////////////Physics///////////////////////

        if (bubbleContactToCharacterBullet){
            PhysicsBody* bodyBubble = (bodyA->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK)
                                        ? bodyA : bodyB;
            PhysicsBody* bodyBullet = (bodyA->getCategoryBitmask() == Constants::BUBBLE_CATEGORY_BITMASK)
                                        ? bodyB : bodyA;
            //Update score's number to label, based in size of bubble
            score += Constants::BUBBLE_BIG_TAG - bodyBubble->getNode()->getTag() + 1;
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

        if (characterBulletContactToBoss){
            PhysicsBody* bodyBullet = (bodyA->getCategoryBitmask() == Constants::CHARACTER_BULLET_CATEGORY_BITMASK)
                                        ? bodyA : bodyB;
            PhysicsBody* bodyBoss = (bodyA->getCategoryBitmask() == Constants::CHARACTER_BULLET_CATEGORY_BITMASK)
                                        ? bodyB : bodyA;
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
            //Decrease boss hp
            auto bossShooted = bodyBoss->getNode();
            for (unsigned int i = 0; i < bosses->size(); ++i){
                auto bossMatched = bosses->at(i);
                if (bossShooted == bossMatched->getBossSprite()){
                    if (bossMatched->getHP() > 0){
                        //Play soundtrack
                        if (SoundManager::isPlayingSoundtrack())
                            SoundManager::playSoundtrack(SoundManager::EXPLOSION_AUDIO);
                        score += Constants::BOSS_SCORE;
                        //Set score to scoreLabel
                        scoreLabel->setString(std::to_string(score));
                        bossMatched->setHP(bossMatched->getHP() - 1);
                    }
                    break;
                }
            }
            characterBulletContactToBoss = false;
        }
    }
    else {
        //Game over

        //Play soundtrack
        if (SoundManager::isPlayingSoundtrack())
            SoundManager::playSoundtrack(SoundManager::DIE_AUDIO);

        Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
        character->stop();

        for (unsigned i = 0; i < bosses->size(); ++i)
            bosses->at(i)->stop();

        this->unscheduleUpdate();

        scoreLabel->removeFromParent();
        controller->removeFromParent();

        endLayer = EndLayer::create();
        endLayer->setLabel(score);
        this->addChild(endLayer, 10);
    }
}

void NormalMap::close(Ref* sender){
    delete character;
    delete bubble;
    delete bulletPackage;
    delete infBulletPackage;

    while (!bosses->empty()){
        auto firstBoss = bosses->at(0);
        bosses->erase(bosses->begin());
        delete firstBoss;
    }
    delete bosses;
    this->removeFromParentAndCleanup(true);
}

void NormalMap::setController(ControlLayer* controller){
    this->controller = controller;
}

bool NormalMap::onContactBegin(cocos2d::PhysicsContact &contact){
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
    else if ((bodyA->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::BOSS_CATEGORY_BITMASK) ||
        (bodyA->getCategoryBitmask() == Constants::BOSS_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK)){
            gameOver = true;
    }
    else if ((bodyA->getCategoryBitmask() == Constants::CHARACTER_BULLET_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::BOSS_CATEGORY_BITMASK) ||
        (bodyA->getCategoryBitmask() == Constants::BOSS_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::CHARACTER_BULLET_CATEGORY_BITMASK)){
            this->bodyA = bodyA;
            this->bodyB = bodyB;
            characterBulletContactToBoss = true;
    }
    else if ((bodyA->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::BOSS_BULLET_CATEGORY_BITMASK) ||
        (bodyA->getCategoryBitmask() == Constants::BOSS_BULLET_CATEGORY_BITMASK &&
        bodyB->getCategoryBitmask() == Constants::CHARACTER_CATEGORY_BITMASK)){
            gameOver = true;
    }
    else {
    }
    return true;
}
