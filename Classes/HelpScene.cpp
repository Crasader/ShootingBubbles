/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelpScene.h"
#include "SimpleAudioEngine.h"
#include "Constants.h"
#include "ui/CocosGUI.h"
#include "MenuScene.h"
#include "DataManager.h"

Scene* HelpScene::createScene()
{
    return HelpScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelpScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    auto background = Sprite::createWithSpriteFrameName(Constants::HELP_BACKGROUND);
    background->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    this->addChild(background);

    auto overlay = Sprite::createWithSpriteFrameName(Constants::OVERLAY_BACKGROUND);
    overlay->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    overlay->setOpacity(Constants::OPACITY);
    this->addChild(overlay);

    container = Node::create();
    container->setContentSize(size);
    container->setPosition(origin.x, origin.y );

    this->addChild(container);

    auto buttonNext = ui::Button::create();
    buttonNext->loadTextures(Constants::NEXT_BUTTON_0, Constants::NEXT_BUTTON_1, Constants::NEXT_BUTTON_1, ui::Widget::TextureResType::PLIST);
    Size buttonSize = buttonNext->getContentSize();
    buttonNext->setPosition(Point(origin.x + size.width - buttonSize.width * 0.5f - Constants::CONTROLLER_PANEL_MARGIN,
                            origin.y + buttonSize.height * 0.5f + Constants::CONTROLLER_PANEL_MARGIN));
    this->addChild(buttonNext);

    //Play background music
    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        //Play background music if ok
        if (dataManager->isBGMusicIsTurningOn() && !CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
                Constants::BACKGROUND_MUSIC.c_str(), true);
        dataManager->close();
    }
    delete dataManager;

    initStage0();

    //Events
    buttonNext->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                DataManager* dataManager = new DataManager();
                if (dataManager->init()){
                    //Play sountrack if ok
                    if (dataManager->isAudioIsTurningOn())
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                        Constants::BUTTON_CLICK_AUDIO.c_str());
                    dataManager->close();
                }
                delete dataManager;
                ++stage;
                switch (stage){
                    case 1:
                        initStage1(); break;
                    case 2:
                        initStage2(); break;
                    case 3:
                        initStage3(); break;
                    case 4:
                    {
                        //Turn off play this scene next time
                        DataManager* dataManager = new DataManager();
                        if (dataManager->init()){
                            dataManager->setTutorialShowPlaying(false);
                            dataManager->close();
                        }
                        //Change to menu Scene
                        auto nextScene = MenuScene::createScene();
                        auto transition = TransitionFade::create(1.0f, nextScene);
                        Director::getInstance()->replaceScene(transition);
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case ui::Widget::TouchEventType::MOVED:
                break;
            case ui::Widget::TouchEventType::ENDED:
                break;
            case ui::Widget::TouchEventType::CANCELED:
                break;
            default:
                break;
        }
    });

    stage = 0;

    return true;
}

void HelpScene::initStage0(){
    container->removeAllChildren();
    auto labelTitle = Label::createWithTTF("QUICK TUTORIAL", Constants::TITLE_FONT_PATH, Constants::TITLE_FONT_SIZE);
    labelTitle->setPosition(Vec2(origin.x + size.width * 0.5f, origin.y + size.height * 0.7f));
    labelTitle->setColor(Color3B(255, 255, 255));
    labelTitle->runAction(RepeatForever::create(JumpBy::create(1.0f, Vec2(0.0f, 0.0f), 20.0f, 1)));

    container->addChild(labelTitle);

    auto labelNext = Label::createWithTTF("This will be over soon.", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelNext->setPosition(Vec2(origin.x + size.width * 0.5f, origin.y + size.height * 0.4f));
    labelNext->setColor(Color3B(255, 255, 255));
    labelNext->runAction(RepeatForever::create(JumpBy::create(1.0f, Vec2(0.0f, 0.0f), -20.0f, 1)));

    container->addChild(labelNext);
}

void HelpScene::initStage1(){
    container->removeAllChildren();
    auto labelCharacter = Label::createWithTTF("Your character", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelCharacter->setPosition(Vec2(origin.x + size.width * 0.3f, origin.y + size.height * 0.5f));
    labelCharacter->setColor(Color3B(255, 255, 255));

    auto arrowCharacter = Sprite::createWithSpriteFrameName(Constants::ARROW_RIGHT);
    arrowCharacter->setPosition(Vec2(origin.x + size.width * 0.44f, origin.y + size.height * 0.5f));
    arrowCharacter->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(0.5f, Vec2(20, 0)),
        MoveBy::create(0.5f, Vec2(-20, 0)),
        nullptr
    )));

    container->addChild(arrowCharacter);
    container->addChild(labelCharacter);

    auto labelBubble = Label::createWithTTF("This is a bubble.\nDodge it!", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelBubble->setPosition(Vec2(origin.x + size.width * 0.6f, origin.y + size.height * 0.4f));
    labelBubble->setColor(Color3B(255, 255, 255));

    auto arrowBubble = Sprite::createWithSpriteFrameName(Constants::ARROW_RIGHT);
    arrowBubble->setPosition(Vec2(origin.x + size.width * 0.74f, origin.y + size.height * 0.4f));
    arrowBubble->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(0.5f, Vec2(20, 0)),
        MoveBy::create(0.5f, Vec2(-20, 0)),
        nullptr
    )));

    container->addChild(arrowBubble);
    container->addChild(labelBubble);

    auto labelPackage = Label::createWithTTF("Collect this\nto power up", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelPackage->setPosition(Vec2(origin.x + size.width * 0.44f, origin.y + size.height * 0.65f));
    labelPackage->setColor(Color3B(255, 255, 255));

    auto arrowPackage = Sprite::createWithSpriteFrameName(Constants::ARROW_LEFT);
    arrowPackage->setPosition(Vec2(origin.x + size.width * 0.3f, origin.y + size.height * 0.65f));
    arrowPackage->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(0.5f, Vec2(-20, 0)),
        MoveBy::create(0.5f, Vec2(20, 0)),
        nullptr
    )));

    container->addChild(arrowPackage);
    container->addChild(labelPackage);
}

void HelpScene::initStage2(){
    container->removeAllChildren();

    auto labelController = Label::createWithTTF("Move this to \ncontrol the \ncharacter", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelController->setPosition(Vec2(origin.x + size.width * 0.32f, origin.y + size.height * 0.25f));
    labelController->setColor(Color3B(255, 255, 255));

    auto arrowController = Sprite::createWithSpriteFrameName(Constants::ARROW_LEFT);
    arrowController->setPosition(Vec2(origin.x + size.width * 0.2f, origin.y + size.height * 0.25f));
    arrowController->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(0.5f, Vec2(-20, 0)),
        MoveBy::create(0.5f, Vec2(20, 0)),
        nullptr
    )));

    container->addChild(arrowController);
    container->addChild(labelController);

    auto labelShooting = Label::createWithTTF("Tap this to \nshoot", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelShooting->setPosition(Vec2(origin.x + size.width * 0.7f, origin.y + size.height * 0.1f));
    labelShooting->setColor(Color3B(255, 255, 255));

    auto arrowShooting = Sprite::createWithSpriteFrameName(Constants::ARROW_RIGHT);
    arrowShooting->setPosition(Vec2(origin.x + size.width * 0.8f, origin.y + size.height * 0.1f));
    arrowShooting->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(0.5f, Vec2(20, 0)),
        MoveBy::create(0.5f, Vec2(-20, 0)),
        nullptr
    )));

    container->addChild(arrowShooting);
    container->addChild(labelShooting);

    auto labelBulletBar = Label::createWithTTF("Now your character\n has 4/5 bullets", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelBulletBar->setPosition(Vec2(origin.x + size.width * 0.7f, origin.y + size.height * 0.35f));
    labelBulletBar->setColor(Color3B(255, 255, 255));

    auto arrowBullerBar = Sprite::createWithSpriteFrameName(Constants::ARROW_RIGHT);
    arrowBullerBar->setPosition(Vec2(origin.x + size.width * 0.85f, origin.y + size.height * 0.35f));
    arrowBullerBar->runAction(RepeatForever::create(Sequence::create(
        MoveBy::create(0.5f, Vec2(20, 0)),
        MoveBy::create(0.5f, Vec2(-20, 0)),
        nullptr
    )));

    container->addChild(labelBulletBar);
    container->addChild(arrowBullerBar);
}

void HelpScene::initStage3(){
    container->removeAllChildren();
    auto labelOver = Label::createWithTTF("YOU CAN PLAY NOW !", Constants::TITLE_FONT_PATH, Constants::TITLE_FONT_SIZE);
    labelOver->setPosition(Vec2(origin.x + size.width * 0.5f, origin.y + size.height * 0.7f));
    labelOver->setColor(Color3B(255, 255, 255));
    labelOver->runAction(RepeatForever::create(JumpBy::create(1.0f, Vec2(0.0f, 0.0f), 20.0f, 1)));

    container->addChild(labelOver);

    auto labelNext = Label::createWithTTF("Tap Next to play \n ", Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    labelNext->setPosition(Vec2(origin.x + size.width * 0.5f, origin.y + size.height * 0.4f));
    labelNext->setColor(Color3B(255, 255, 255));
    labelNext->runAction(RepeatForever::create(JumpBy::create(1.0f, Vec2(0.0f, 0.0f), -20.0f, 1)));

    container->addChild(labelNext);
}

void HelpScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
