#include "ControlLayer.h"
#include "GameLibrary.h"
#include <cmath>

USING_NS_CC;

ControlLayer::ControlLayer(){

}
ControlLayer::~ControlLayer(){

}

bool ControlLayer::init(){

    if (!Layer::init()){
        return false;
    }

    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    this->direction = 0;
    this->isShoot = false;

    Sprite* controllerPanel = Sprite::createWithSpriteFrameName(Constants::ROUND_PANEL_PATH);
    auto panelSize = controllerPanel->getBoundingBox().size;
    controllerPanel->setPosition(origin.x + panelSize.width * 0.5f + Constants::CONTROLLER_PANEL_MARGIN,
                                origin.y + panelSize.height * 0.5f + Constants::CONTROLLER_PANEL_MARGIN);
    controllerPanel->setOpacity(100);
    controllerPanel->setTag(Constants::CONTROLLER_PANEL_ID);
    this->addChild(controllerPanel);

    Sprite* controller = Sprite::createWithSpriteFrameName(Constants::ROUND_BUTTON_PATH);
    controller->setPosition(origin.x + panelSize.width * 0.5f + Constants::CONTROLLER_PANEL_MARGIN,
                            origin.y + panelSize.height * 0.5f + Constants::CONTROLLER_PANEL_MARGIN);
    controller->setOpacity(100);
    controller->setTag(Constants::CONTROLLER_ID);
    this->addChild(controller);

    Sprite* shootingPanel = Sprite::createWithSpriteFrameName(Constants::SHOOTING_PANEL_PATH);
    auto shootingPanelSize = shootingPanel->getBoundingBox().size;
    shootingPanel->setPosition(origin.x + size.width - shootingPanelSize.width * 0.5f - Constants::CONTROLLER_PANEL_MARGIN,
                            origin.y + shootingPanelSize.height * 0.5f + Constants::CONTROLLER_PANEL_MARGIN);
    shootingPanel->setTag(Constants::SHOOTING_PANEL_ID);
    shootingPanel->setOpacity(100);
    this->addChild(shootingPanel);

    //Bulet bar
    auto barContainerSprite = Sprite::createWithSpriteFrameName(Constants::BULLET_BAR_CONTAINER_PATH);
    auto barContainerSpriteSize = barContainerSprite->getBoundingBox().size;
    barContainerSprite->setPosition(origin.x + size.width - barContainerSpriteSize.width * 0.5f - Constants::CONTROLLER_PANEL_MARGIN,
                            origin.y + barContainerSpriteSize.height * 0.5f + Constants::CONTROLLER_PANEL_MARGIN);
    barContainerSprite->setOpacity(Constants::BULLET_BAR_CONTAINER_OPACITY);
    this->addChild(barContainerSprite);

    auto sprite = Sprite::createWithSpriteFrameName(Constants::BULLET_BAR_PATH);
    sprite->setOpacity(Constants::BULLET_BAR_OPACITY);

    bulletBar = ProgressTimer::create(sprite);
    bulletBar->setMidpoint(Vec2(0.9999999f, 0.0000001f));
    bulletBar->setType(ProgressTimer::Type::RADIAL);
    bulletBar->setColor(Color3B::RED);
    bulletBar->setPosition(barContainerSprite->getPosition());

    bulletBar->setPercentage(100.0f);

    this->addChild(bulletBar);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(ControlLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ControlLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ControlLayer::onTouchEnded, this);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void ControlLayer::close(Ref* sender){
    this->removeFromParentAndCleanup(true);
}

bool ControlLayer::onTouchBegan(Touch* touch, Event* event){
    auto touchPoint = touch->getLocation();
    auto shootingPanel = event->getCurrentTarget()->getChildByTag(Constants::SHOOTING_PANEL_ID);

    if (shootingPanel->getBoundingBox().containsPoint(touchPoint)){
        isShoot = true;
    }
    else {
        // Controller
        auto controllerPanel = event->getCurrentTarget()->getChildByTag(Constants::CONTROLLER_PANEL_ID);
        auto controller = event->getCurrentTarget()->getChildByTag(Constants::CONTROLLER_ID);
        controllerPanel->setPosition(touchPoint);
        controller->setPosition(touchPoint);
    }

    return true;
}

void ControlLayer::onTouchMoved(Touch* touch, Event* event){

    auto controller = event->getCurrentTarget()->getChildByTag(Constants::CONTROLLER_ID);
    auto touchPoint = touch->getLocation();

    if (controller->getBoundingBox().containsPoint(touchPoint)){
        //Make controller in controllerPanel
        auto controllerPanel = event->getCurrentTarget()->getChildByTag(Constants::CONTROLLER_PANEL_ID);
        auto limitedZone = controllerPanel->getBoundingBox().size;
        auto controllerPanelPos = controllerPanel->getPosition();

        Vec2 position = touchPoint;
        if (position.x < controllerPanelPos.x + origin.x - limitedZone.width)
            position.x = controllerPanelPos.x + origin.x - limitedZone.width;
        else if (position.x > origin.x + controllerPanelPos.x + 2 * Constants::CONTROLLER_PANEL_MARGIN + limitedZone.width)
            position.x = origin.x + controllerPanelPos.x + limitedZone.width + 2*Constants::CONTROLLER_PANEL_MARGIN;

        if (position.y < origin.y + controllerPanelPos.y - limitedZone.height)
            position.y = origin.y + controllerPanelPos.y - limitedZone.height;
        else if (position.y > origin.y + controllerPanelPos.y + 2*Constants::CONTROLLER_PANEL_MARGIN + limitedZone.height)
            position.y = origin.y + controllerPanelPos.y + limitedZone.height + 2*Constants::CONTROLLER_PANEL_MARGIN;
        controller->setPosition(position);

        //Set value of controller (up, down, left, right) to direction
        direction = directionOf(touchPoint, controllerPanelPos);
    }
}

 void ControlLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
     //Make controller back to the middle
     auto touchPoint = touch->getLocation();
     auto shootingPanel = event->getCurrentTarget()->getChildByTag(Constants::SHOOTING_PANEL_ID);

     if (!shootingPanel->getBoundingBox().containsPoint(touchPoint)){
         auto controllerPanelPos = event->getCurrentTarget()->getChildByTag(Constants::CONTROLLER_PANEL_ID)->getPosition();
         auto controller = event->getCurrentTarget()->getChildByTag(Constants::CONTROLLER_ID);
         controller->setPosition(origin.x + controllerPanelPos.x, origin.y + controllerPanelPos.y);
     }
 }

int ControlLayer::directionOf(Vec2 point, Vec2 centerPoint){
    float deltaX = point.x - centerPoint.x;
    float deltaY = point.y - centerPoint.y;
    if (deltaX < 0){
        //Left, Half Up, Half Down
        if (deltaY < 0){
            //Half Left, Half Down
            float ratio = atan(abs(deltaY / deltaX)) * 180 / Constants::PI;
            if (ratio <= 45.0f){
                //Left
                return Constants::DIR_LEFT;
            }
            else {
                return Constants::DIR_DOWN;
            }
        }
        else {
            // Haft left, half up
            float ratio = atan(abs(deltaY / deltaX)) * 180 / Constants::PI;
            if (ratio <= 45.0f){
                //Left
                return Constants::DIR_LEFT;
            }
            else {
                return Constants::DIR_UP;
            }
        }
    }
    else {
        //Right, Half Up, Half Down
        if (deltaY < 0){
            //Half Right, Half Down
            float ratio = atan(abs(deltaY / deltaX)) * 180 / Constants::PI;
            if (ratio <= 45.0f){
                //Right
                return Constants::DIR_RIGHT;
            }
            else {
                return Constants::DIR_DOWN;
            }
        }
        else {
            // Haft right, half up
            float ratio = atan(abs(deltaY / deltaX)) * 180 / Constants::PI;
            if (ratio <= 45.0f){
                //Right
                return Constants::DIR_RIGHT;
            }
            else {
                return Constants::DIR_UP;
            }
        }
    }
}

void ControlLayer::changeBulletBarColor(Color3B color){
    bulletBar->setColor(color);
}

void ControlLayer::setBulletBarNum(int num){
    bulletBar->setPercentage(75.0f + 5.0f * num);
}

int ControlLayer::getDirection(){
    return this->direction;
}

bool ControlLayer::getShooting(){
    return this->isShoot;
}

void ControlLayer::setShooting(bool value){
    this->isShoot = value;
}
