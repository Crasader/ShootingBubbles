#include "EndLayer.h"
#include "../MenuScene/MenuScene.h"
#include "../../Libs/GameLibrary.h"
#include <ctime>

USING_NS_CC;

EndLayer::EndLayer(){

}
EndLayer::~EndLayer(){

}

bool EndLayer::init(){

    if (!Layer::init()){
        return false;
    }

    Size size = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite* overlay = Sprite::createWithSpriteFrameName(Constants::OVERLAY_BACKGROUND);
    overlay->setOpacity(Constants::OPACITY);
    overlay->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    this->addChild(overlay);

    Sprite* infoBackground = Sprite::createWithSpriteFrameName(Constants::WHITE_CONTAINER_PATH);
    infoBackground->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.25f);

    Size infoBackgroundSize = infoBackground->getContentSize();

    auto label = Label::createWithTTF(Constants::GAME_OVER_TITLE, Constants::TITLE_FONT_PATH, Constants::TITLE_FONT_SIZE);
    label->setPosition(infoBackgroundSize.width / 2, infoBackgroundSize.height * 0.75f);
    label->setColor(Color3B(88, 105, 132));
    infoBackground->addChild(label);

    scoreLabel = Label::createWithTTF("Score: ", Constants::DEFAULT_FONT_PATH, Constants::RECORD_FONT_SIZE);
    scoreLabel->setPosition(infoBackgroundSize.width * 0.5f, infoBackgroundSize.height * 0.55f);
    scoreLabel->setColor(Color3B(88, 105, 132));
    infoBackground->addChild(scoreLabel);

    highScoreLabel = Label::createWithTTF("Your best: 10", Constants::DEFAULT_FONT_PATH, Constants::RECORD_FONT_SIZE);
    highScoreLabel->setPosition(infoBackgroundSize.width * 0.5f, infoBackgroundSize.height * 0.45f);
    highScoreLabel->setColor(Color3B(88, 105, 132));
    infoBackground->addChild(highScoreLabel);

    auto buttonPlay = ui::Button::create();
    buttonPlay->loadTextures(Constants::RESTART_BUTTON_0, Constants::RESTART_BUTTON_1, Constants::RESTART_BUTTON_1, ui::Widget::TextureResType::PLIST);
    buttonPlay->setPosition(Vec2(infoBackgroundSize.width * 0.5f, infoBackgroundSize.height * 0.2f));
    infoBackground->addChild(buttonPlay);

    this->addChild(infoBackground);

    auto sceneAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y + size.height / 2));
    infoBackground->runAction(sceneAnimation);

    //Listener:
    buttonPlay->addTouchEventListener([](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Playing sountrack
                if (SoundManager::isPlayingSoundtrack())
                    SoundManager::playSoundtrack(SoundManager::BUTTON_CLICK_AUDIO);
                DataManager* dataManager = new DataManager();
                if (dataManager->init()){
                    //Increase playing time
                    dataManager->setCountPlayingTimes(dataManager->countPlayingTimes() + 1);
                    dataManager->close();
                }

                delete dataManager;

                auto nextScene = MenuScene::createScene();
                auto transition = TransitionFade::create(1.0f, nextScene);
                Director::getInstance()->replaceScene(transition);
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

    return true;
}

void EndLayer::setLabel(int score){

    //Write score to database
    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        dataManager->appendScore(score);
    }

    //Set label
    scoreLabel->setString("Score: " + std::to_string(score));
    highScoreLabel->setString("Your best: " + std::to_string(dataManager->getHighestRecord()));

    //Close database
    dataManager->close();
    delete dataManager;
}

void EndLayer::close(Ref* sender){

    this->removeFromParentAndCleanup(true);
}
