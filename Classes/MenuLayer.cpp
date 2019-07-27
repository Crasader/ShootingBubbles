#include "MenuLayer.h"
#include "GameLibrary.h"
#include "PlayScene.h"
#include "HelpScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

MenuLayer::MenuLayer(){

}

MenuLayer::~MenuLayer(){

}

bool MenuLayer::init(){

    if (!Layer::init()){
        return false;
    }

    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Sprite* overlay = Sprite::createWithSpriteFrameName(Constants::OVERLAY_BACKGROUND);
    overlay->setOpacity(Constants::OPACITY);
    overlay->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    this->addChild(overlay);

    isPlayingSoundtrack = true;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        top5Record = dataManager->getTop5Record();
        bool isPlayingBackgroundMusic = dataManager->isBGMusicIsTurningOn();
        isPlayingSoundtrack = dataManager->isAudioIsTurningOn();
        //Play background music if ok
        if (isPlayingBackgroundMusic && !CocosDenshion::SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
                Constants::BACKGROUND_MUSIC.c_str(), true);

        dataManager->close();
    }

    delete dataManager;

    initSettingSprite();
    initStatsSprite();
    initPlaySprite();

    return true;
}

void MenuLayer::initPlaySprite(){
    playSprite = Sprite::createWithSpriteFrameName(Constants::WHITE_CONTAINER_PATH);
    playSprite->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);

    Size playSpriteSize = playSprite->getContentSize();

    auto label = Label::createWithTTF(Constants::GAME_TITLE, Constants::TITLE_FONT_PATH, Constants::TITLE_FONT_SIZE);
    label->setPosition(playSpriteSize.width / 2, playSpriteSize.height * 0.7f);
    label->setColor(Color3B(88, 105, 132));
    playSprite->addChild(label);

    auto buttonPlay = ui::Button::create();
    initPlayButton(playSpriteSize, buttonPlay);

    auto buttonSetting = ui::Button::create();
    initSettingButton(playSpriteSize, buttonSetting);

    auto buttonStats = ui::Button::create();
    initStatsButton(playSpriteSize, buttonStats);

    this->addChild(playSprite);
}

void MenuLayer::initPlayButton(Size containerSize, ui::Button*& button){
    button->loadTextures(Constants::PLAY_BUTTON_0, Constants::PLAY_BUTTON_1, Constants::PLAY_BUTTON_1, ui::Widget::TextureResType::PLIST);
    button->setPosition(Vec2(containerSize.width * 0.5f, containerSize.height * 0.2f));
    playSprite->addChild(button);

    //Events
    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());
                //Change to Play Scene
                auto nextScene = PlayScene::createScene();
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
}

void MenuLayer::initSettingButton(Size containerSize, ui::Button*& button){
    button->loadTextures(Constants::SETTING_BUTTON_0, Constants::SETTING_BUTTON_1, Constants::SETTING_BUTTON_1, ui::Widget::TextureResType::PLIST);
    button->setPosition(Vec2(containerSize.width * 0.8f, containerSize.height * 0.2f));
    playSprite->addChild(button);

    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());

                //Play sprite is out
                auto outAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y - size.height * 0.5f));
                playSprite->runAction(outAnimation);

                //Setting sprite is in
                settingSprite->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.5f);
                auto inAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y + size.height / 2));
                settingSprite->runAction(inAnimation);
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
}

void MenuLayer::initStatsButton(Size containerSize, ui::Button*& button){

    button->loadTextures(Constants::STATS_BUTTON_0, Constants::STATS_BUTTON_1, Constants::STATS_BUTTON_1, ui::Widget::TextureResType::PLIST);
    button->setPosition(Vec2(containerSize.width * 0.2f, containerSize.height * 0.2f));
    playSprite->addChild(button);

    button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());

                //Play sprite is out
                auto outAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y - size.height * 0.5f));
                playSprite->runAction(outAnimation);

                //Stats sprite is in
                statsSprite->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.5f);
                auto inAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y + size.height / 2));
                statsSprite->runAction(inAnimation);
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
}

void MenuLayer::initSettingSprite(){
    settingSprite = Sprite::createWithSpriteFrameName(Constants::SETTING_BACKGROUND_PATH);
    settingSprite->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.5f);

    Size settingSpriteSize = settingSprite->getContentSize();

    auto label = Label::createWithTTF(Constants::SETTING_SPRITE_TITLE, Constants::TITLE_FONT_PATH, Constants::TITLE_FONT_SIZE);
    label->setPosition(settingSpriteSize.width / 2, settingSpriteSize.height * 0.88f);
    label->setColor(Color3B(88, 105, 132));
    settingSprite->addChild(label);

    auto closeButton = ui::Button::create();
    closeButton->loadTextures(Constants::CLOSE_BUTTON_0, Constants::CLOSE_BUTTON_1, Constants::CLOSE_BUTTON_1, ui::Widget::TextureResType::PLIST);
    closeButton->setPosition(Vec2(settingSpriteSize.width * 0.92f, settingSpriteSize.height * 0.9f));
    settingSprite->addChild(closeButton);

    closeButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());

                //Stats sprite is out
                auto outAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y - size.height * 0.5f));
                settingSprite->runAction(outAnimation);

                //Play sprite is in
                playSprite->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.5f);
                auto inAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y + size.height / 2));
                playSprite->runAction(inAnimation);
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

    TabView* tabView = new TabView(settingSprite);
    tabView->setTabsPos(TabView::TABS_POS_UP);
    tabView->setPosition(Vec2(settingSpriteSize.width * 0.5f, settingSpriteSize.height * 0.325f));
    tabView->setNumOfTabs(Constants::NUM_OF_SETTING_TABS);
    tabView->setPanelColor(Color3B(245, 249, 255));
    for (int i = 0; i < Constants::NUM_OF_SETTING_TABS; ++i){
        tabView->pushTab(Constants::SETTING_TAB_HEADER_PATH + std::to_string(i) + "0.png",
                         Constants::SETTING_TAB_HEADER_PATH + std::to_string(i) + "1.png",
                        Constants::SETTING_LAYOUT_PATH,
                        true);
    }

    initCharacterSkinTab(tabView->getPanelOfTab(0));
    initMusicTab(tabView->getPanelOfTab(1));
    initMapTab(tabView->getPanelOfTab(2));
    initInfoTab(tabView->getPanelOfTab(3));

    this->addChild(settingSprite);
}

void MenuLayer::initCharacterSkinTab(Node* container){

    // Get necessary data from database
    int characterColor = 0;
    int sumOfRecords = 0;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        characterColor = dataManager->getCharacterColor();
        sumOfRecords = dataManager->getSumOfRecords();
        dataManager->close();
    }
    delete dataManager;

    Size containerSize = container->getContentSize();

    auto totalScoreLabel = Label::createWithTTF("Total score: " + std::to_string(sumOfRecords), Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    totalScoreLabel->setPosition(Vec2(containerSize.width * 0.5, containerSize.height * 0.85f));
    totalScoreLabel->setColor(Color3B(88, 105, 132));
    container->addChild(totalScoreLabel);

    auto pageView = ui::PageView::create();
    pageView->setAnchorPoint(Point(0.5f, 0.5f));
    pageView->setPosition(Vec2(containerSize.width * 0.5, containerSize.height * 0.4f));
    container->addChild(pageView);

    Size slidePanelSize = Size(0, 0);
    int numberOfUnlockedSkin = Constants::NUM_OF_FREE_SKINS + sumOfRecords / Constants::SUM_SCORE_FOR_SKIN;

    for (int i = 0; i < Constants::NUM_OF_CHARACTER_SKIN; ++i) {
        auto page = ui::Layout::create();

        page->setBackGroundImage(Constants::SLIDE_PANEL_PATH, ui::Widget::TextureResType::PLIST);
        slidePanelSize = page->getBackGroundImageTextureSize();

        auto charSkin = Sprite::createWithSpriteFrameName(Constants::CHARACTER_PATH + std::to_string(i) + ".png");
        charSkin->setPosition(slidePanelSize / 2 * Constants::DISTANCE_BETWEEN_SLIDES);
        page->addChild(charSkin);

        if (i >= numberOfUnlockedSkin){
            auto blackMasked = Sprite::createWithSpriteFrameName("slidePanelMasked.png");
            blackMasked->setOpacity(80.0f);
            blackMasked->setPosition(slidePanelSize / 2 * Constants::DISTANCE_BETWEEN_SLIDES);

            int scoreNeededToUnlock = Constants::SUM_SCORE_FOR_SKIN * (i - Constants::NUM_OF_FREE_SKINS + 1);

            auto scoreUnlock = Label::createWithTTF(std::to_string(scoreNeededToUnlock), Constants::DEFAULT_FONT_BOLD_PATH, Constants::SLIDE_FONT_SIZE);
            scoreUnlock->setPosition(blackMasked->getContentSize() / 2);
            scoreUnlock->setColor(Color3B(255,0,0));
            blackMasked->addChild(scoreUnlock);

            page->addChild(blackMasked);
        }
        else {
            auto rotate = RotateBy::create(Constants::CHARACTER_ROTATE_SPEED, 360.0f);
            auto action = RepeatForever::create(rotate);
            charSkin->runAction(action);
        }

        pageView->insertPage(page, i);
    }

    pageView->setContentSize(slidePanelSize * Constants::DISTANCE_BETWEEN_SLIDES);

    pageView->setCurrentPageIndex(characterColor);

    auto nextSlideButton = ui::Button::create();
    nextSlideButton->loadTextures(Constants::NEXT_SLIDE_BUTTON_PATH, Constants::NEXT_SLIDE_BUTTON_PATH, Constants::NEXT_SLIDE_BUTTON_PATH, ui::Widget::TextureResType::PLIST);
    nextSlideButton->setPosition(Vec2(containerSize.width * 0.6f + slidePanelSize.width / 2,
                                    containerSize.height * 0.4f));
    if (characterColor == Constants::NUM_OF_CHARACTER_SKIN - 1) nextSlideButton->setVisible(false);
    container->addChild(nextSlideButton);

    auto previousSlideButton = ui::Button::create();
    previousSlideButton->loadTextures(Constants::PREVIOUS_SLIDE_BUTTON_PATH, Constants::PREVIOUS_SLIDE_BUTTON_PATH, Constants::PREVIOUS_SLIDE_BUTTON_PATH, ui::Widget::TextureResType::PLIST);
    previousSlideButton->setPosition(Vec2(containerSize.width * 0.4f - slidePanelSize.width / 2,
                                    containerSize.height * 0.4f));
    if (characterColor == 0) previousSlideButton->setVisible(false);
    container->addChild(previousSlideButton);

    nextSlideButton->addTouchEventListener([&, pageView, nextSlideButton, previousSlideButton, numberOfUnlockedSkin]
        (Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                int currentPage = pageView->getCurrentPageIndex();
                if (currentPage < Constants::NUM_OF_CHARACTER_SKIN - 1){
                    //Play sountrack
                    if (isPlayingSoundtrack)
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                        Constants::BUTTON_CLICK_AUDIO.c_str());
                    ++currentPage;
                    pageView->scrollToPage(currentPage);
                    previousSlideButton->setVisible(true);
                }

                if (currentPage == Constants::NUM_OF_CHARACTER_SKIN - 1){
                    nextSlideButton->setVisible(false);
                }

                if (currentPage < numberOfUnlockedSkin){
                    DataManager* dataManager = new DataManager();
                    if (dataManager->init()){
                        dataManager->changeCharacterColor(currentPage);
                        dataManager->close();
                    }
                    delete dataManager;
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

    previousSlideButton->addTouchEventListener([&, pageView, nextSlideButton, previousSlideButton, numberOfUnlockedSkin]
        (Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                int currentPage = pageView->getCurrentPageIndex();
                if (currentPage > 0){
                    //Play sountrack
                    if (isPlayingSoundtrack)
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                        Constants::BUTTON_CLICK_AUDIO.c_str());
                    --currentPage;
                    pageView->scrollToPage(currentPage);
                    nextSlideButton->setVisible(true);
                }

                if (currentPage == 0){
                    previousSlideButton->setVisible(false);
                }

                if (currentPage < numberOfUnlockedSkin){
                    DataManager* dataManager = new DataManager();
                    if (dataManager->init()){
                        dataManager->changeCharacterColor(currentPage);
                        dataManager->close();
                    }
                    delete dataManager;
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

    pageView->addEventListener([&, numberOfUnlockedSkin, nextSlideButton, previousSlideButton](Ref* sender, ui::PageView::EventType type){
        if (type == ui::PageView::EventType::TURNING) {
            auto pageView = dynamic_cast<ui::PageView*>(sender);

            int currentPage = pageView->getCurrentPageIndex();

            if (currentPage == Constants::NUM_OF_CHARACTER_SKIN - 1){
                nextSlideButton->setVisible(false);
            }
            else if (currentPage == 0){
                previousSlideButton->setVisible(false);
            }
            else {
                nextSlideButton->setVisible(true);
                previousSlideButton->setVisible(true);
            }

            if (currentPage < numberOfUnlockedSkin){
                DataManager* dataManager = new DataManager();
                if (dataManager->init()){
                    dataManager->changeCharacterColor(currentPage);
                    dataManager->close();
                }
                delete dataManager;
            }
        }
    });
}

void MenuLayer::initMapTab(Node* container){
    // Get necessary data from database
    int highestRecord = 0;
    int currentMap = 0;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        highestRecord = dataManager->getHighestRecord();
        currentMap = dataManager->getMap();
        dataManager->close();
    }
    delete dataManager;

    Size containerSize = container->getContentSize();

    auto recordLabel = Label::createWithTTF("Record: " + std::to_string(highestRecord), Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    recordLabel->setPosition(Vec2(containerSize.width * 0.5, containerSize.height * 0.85f));
    recordLabel->setColor(Color3B(88, 105, 132));
    container->addChild(recordLabel);

    auto pageView = ui::PageView::create();
    pageView->setAnchorPoint(Point(0.5f, 0.5f));
    pageView->setPosition(Vec2(containerSize.width * 0.5, containerSize.height * 0.4f));
    container->addChild(pageView);

    Size slidePanelSize = Size(0, 0);
    int numberOfUnlockedMap = Constants::NUM_OF_FREE_MAP + highestRecord / Constants::SCORE_FOR_NEW_MAP;

    for (int i = 0; i < Constants::NUM_OF_MAP; ++i) {
        auto page = ui::Layout::create();

        page->setBackGroundImage(Constants::MAP_PATH + std::to_string(i) + ".png", ui::Widget::TextureResType::PLIST);
        slidePanelSize = page->getBackGroundImageTextureSize();

        if (i >= numberOfUnlockedMap){
            auto blackMasked = Sprite::createWithSpriteFrameName("slidePanelMasked.png");
            blackMasked->setOpacity(80.0f);
            blackMasked->setPosition(slidePanelSize / 2 * Constants::DISTANCE_BETWEEN_SLIDES);

            int scoreNeededToUnlock = Constants::SCORE_FOR_NEW_MAP * (i - Constants::NUM_OF_FREE_MAP + 1);

            auto scoreUnlockLabel = Label::createWithTTF(std::to_string(scoreNeededToUnlock), Constants::DEFAULT_FONT_BOLD_PATH, Constants::SLIDE_FONT_SIZE);
            scoreUnlockLabel->setPosition(blackMasked->getContentSize() / 2);
            scoreUnlockLabel->setColor(Color3B(255,0,0));
            blackMasked->addChild(scoreUnlockLabel);

            page->addChild(blackMasked);
        }

        pageView->insertPage(page, i);
    }

    pageView->setContentSize(slidePanelSize * Constants::DISTANCE_BETWEEN_SLIDES);

    pageView->setCurrentPageIndex(currentMap);

    auto nextSlideButton = ui::Button::create();
    nextSlideButton->loadTextures(Constants::NEXT_SLIDE_BUTTON_PATH, Constants::NEXT_SLIDE_BUTTON_PATH, Constants::NEXT_SLIDE_BUTTON_PATH, ui::Widget::TextureResType::PLIST);
    nextSlideButton->setPosition(Vec2(containerSize.width * 0.6f + slidePanelSize.width / 2,
                                    containerSize.height * 0.4f));
    if (currentMap == Constants::NUM_OF_MAP - 1) nextSlideButton->setVisible(false);
    container->addChild(nextSlideButton);

    auto previousSlideButton = ui::Button::create();
    previousSlideButton->loadTextures(Constants::PREVIOUS_SLIDE_BUTTON_PATH, Constants::PREVIOUS_SLIDE_BUTTON_PATH, Constants::PREVIOUS_SLIDE_BUTTON_PATH, ui::Widget::TextureResType::PLIST);
    previousSlideButton->setPosition(Vec2(containerSize.width * 0.4f - slidePanelSize.width / 2,
                                    containerSize.height * 0.4f));
    if (currentMap == 0) previousSlideButton->setVisible(false);
    container->addChild(previousSlideButton);

    nextSlideButton->addTouchEventListener([&, pageView, nextSlideButton, previousSlideButton, numberOfUnlockedMap]
        (Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                int currentPage = pageView->getCurrentPageIndex();
                if (currentPage < Constants::NUM_OF_MAP - 1){
                    //Play sountrack
                    if (isPlayingSoundtrack)
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                        Constants::BUTTON_CLICK_AUDIO.c_str());
                    ++currentPage;
                    pageView->scrollToPage(currentPage);
                    previousSlideButton->setVisible(true);
                }

                if (currentPage == Constants::NUM_OF_MAP - 1){
                    nextSlideButton->setVisible(false);
                }

                if (currentPage < numberOfUnlockedMap){
                    DataManager* dataManager = new DataManager();
                    if (dataManager->init()){
                        dataManager->changeMap(currentPage);
                        dataManager->close();
                    }
                    delete dataManager;
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

    previousSlideButton->addTouchEventListener([&, pageView, nextSlideButton, previousSlideButton, numberOfUnlockedMap]
        (Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                int currentPage = pageView->getCurrentPageIndex();
                if (currentPage > 0){
                    //Play sountrack
                    if (isPlayingSoundtrack)
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                        Constants::BUTTON_CLICK_AUDIO.c_str());
                    --currentPage;
                    pageView->scrollToPage(currentPage);
                    nextSlideButton->setVisible(true);
                }

                if (currentPage == 0){
                    previousSlideButton->setVisible(false);
                }

                if (currentPage < numberOfUnlockedMap){
                    DataManager* dataManager = new DataManager();
                    if (dataManager->init()){
                        dataManager->changeMap(currentPage);
                        dataManager->close();
                    }
                    delete dataManager;
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

    pageView->addEventListener([&, numberOfUnlockedMap, nextSlideButton, previousSlideButton](Ref* sender, ui::PageView::EventType type){

        if (type == ui::PageView::EventType::TURNING) {

            auto pageView = dynamic_cast<ui::PageView*>(sender);

            int currentPage = pageView->getCurrentPageIndex();

            if (currentPage == Constants::NUM_OF_MAP - 1){
                nextSlideButton->setVisible(false);
            }
            else if (currentPage == 0){
                previousSlideButton->setVisible(false);
            }
            else {
                nextSlideButton->setVisible(true);
                previousSlideButton->setVisible(true);
            }

            if (currentPage < numberOfUnlockedMap){
                DataManager* dataManager = new DataManager();
                if (dataManager->init()){
                    dataManager->changeMap(currentPage);
                    dataManager->close();
                }
                delete dataManager;
            }
        }
    });
}

void MenuLayer::initMusicTab(Node* container){
    //Load data from data manager

    bool turnOnBG = true, turnOnAudio = true;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        turnOnBG = dataManager->isBGMusicIsTurningOn();
        turnOnAudio = dataManager->isAudioIsTurningOn();
        dataManager->close();
    }

    delete dataManager;

    Size containerSize = container->getContentSize();

    //Sound label:
    auto soundLabel = Label::createWithTTF(Constants::SOUNDTRACKS, Constants::DEFAULT_FONT_BOLD_PATH, Constants::STATS_FONT_SIZE);
    soundLabel->setPosition(containerSize.width * 0.25f, containerSize.height * 0.7f);
    soundLabel->setColor(Color3B(88, 105, 132));
    container->addChild(soundLabel);

    //Audio button

    auto audioButton = ui::CheckBox::create(Constants::AUDIO_BUTTON_0,
                                        Constants::AUDIO_BUTTON_0,
                                        Constants::AUDIO_BUTTON_1,
                                        Constants::AUDIO_BUTTON_1,
                                        Constants::AUDIO_BUTTON_1,
                                        ui::Widget::TextureResType::PLIST);
    audioButton->setPosition(Vec2(containerSize.width * 0.25f, containerSize.height * 0.45f));
    audioButton->setSelected(!turnOnAudio);
    audioButton->setScale(0.75f);

    audioButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type){
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                DataManager* dataManager = new DataManager();
                if (dataManager->init()){
                    isPlayingSoundtrack = !dataManager->isAudioIsTurningOn();
                    dataManager->turnOnAudio(isPlayingSoundtrack);
                    dataManager->close();
                }
                delete dataManager;
                break;
            }
            default:
                break;
        }
    });
    container->addChild(audioButton);

    //Music label:
    auto musicLabel = Label::createWithTTF(Constants::BACKGROUND_MUSICS, Constants::DEFAULT_FONT_BOLD_PATH, Constants::STATS_FONT_SIZE);
    musicLabel->setPosition(containerSize.width * 0.75f, containerSize.height * 0.7f);
    musicLabel->setColor(Color3B(88, 105, 132));
    container->addChild(musicLabel);

    //Music button

    auto musicButton = ui::CheckBox::create(Constants::MUSIC_BUTTON_0,
                                        Constants::MUSIC_BUTTON_0,
                                        Constants::MUSIC_BUTTON_1,
                                        Constants::MUSIC_BUTTON_1,
                                        Constants::MUSIC_BUTTON_1,
                                        ui::Widget::TextureResType::PLIST);
    musicButton->setPosition(Vec2(containerSize.width * 0.75f, containerSize.height * 0.45f));
    musicButton->setSelected(!turnOnBG);
    musicButton->setScale(0.75f);

    musicButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type){
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
            {
                DataManager* dataManager = new DataManager();
                if (dataManager->init()){
                    bool isPlayingBackgroundMusic = dataManager->isBGMusicIsTurningOn();
                    dataManager->turnOnBGMusic(!isPlayingBackgroundMusic);
                    //Play background music after click ok
                    if (!isPlayingBackgroundMusic)
                        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(
                            Constants::BACKGROUND_MUSIC.c_str(), true);
                    else {
                        //Turn off background music
                        CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                    }
                    dataManager->close();
                }
                delete dataManager;

                break;
            }
            default:
                break;
        }
    });

    container->addChild(musicButton);
}

void MenuLayer::initInfoTab(Node* container){
    Size containerSize = container->getContentSize();

    auto author = Label::createWithTTF(Constants::GAME_AUTHOR, Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    author->setPosition(Vec2(containerSize.width / 2, containerSize.height * 0.8f));
    author->setColor(Color3B(88, 105, 132));
    container->addChild(author);

    auto version = Label::createWithTTF("Version: " + Constants::GAME_VERSION, Constants::DEFAULT_FONT_PATH, Constants::NORMAL_FONT_SIZE);
    version->setPosition(Vec2(containerSize.width / 2, containerSize.height * 0.6f));
    version->setColor(Color3B(88, 105, 132));
    container->addChild(version);

    auto privacyButton = ui::Button::create();
    privacyButton->loadTextures(Constants::PRIVACY_POLICY_BUTTON_0, Constants::PRIVACY_POLICY_BUTTON_1, Constants::PRIVACY_POLICY_BUTTON_1, ui::Widget::TextureResType::PLIST);
    privacyButton->setPosition(Vec2(containerSize.width * 0.25f, containerSize.height * 0.25f));
    container->addChild(privacyButton);

    auto helpButton = ui::Button::create();
    helpButton->loadTextures(Constants::HELP_BUTTON_0, Constants::HELP_BUTTON_1, Constants::HELP_BUTTON_1, ui::Widget::TextureResType::PLIST);
    helpButton->setPosition(Vec2(containerSize.width * 0.75f, containerSize.height * 0.25f));
    container->addChild(helpButton);

    helpButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());

                //Change to Help Scene
                auto nextScene = HelpScene::createScene();
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

    privacyButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());
                Application::getInstance()->openURL(Constants::GAME_PRIVACY_POLICY_LINK);
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
}

void MenuLayer::initStatsSprite(){
    statsSprite = Sprite::createWithSpriteFrameName(Constants::WHITE_CONTAINER_PATH);
    statsSprite->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.5f);

    Size statsSpriteSize = statsSprite->getContentSize();

    auto label = Label::createWithTTF(Constants::STATS_SPRITE_TITLE, Constants::TITLE_FONT_PATH, Constants::TITLE_FONT_SIZE);
    label->setPosition(statsSpriteSize.width / 2, statsSpriteSize.height * 0.85f);
    label->setColor(Color3B(88, 105, 132));
    statsSprite->addChild(label);

    auto closeButton = ui::Button::create();
    closeButton->loadTextures(Constants::CLOSE_BUTTON_0, Constants::CLOSE_BUTTON_1, Constants::CLOSE_BUTTON_1, ui::Widget::TextureResType::PLIST);
    closeButton->setPosition(Vec2(statsSpriteSize.width * 0.92, statsSpriteSize.height * 0.9f));
    statsSprite->addChild(closeButton);

    closeButton->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
            {
                //Play sountrack
                if (isPlayingSoundtrack)
                CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(
                    Constants::BUTTON_CLICK_AUDIO.c_str());

                //Stats sprite is out
                auto outAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y - size.height * 0.5f));
                statsSprite->runAction(outAnimation);

                //Play sprite is in
                playSprite->setPosition(origin.x + size.width / 2, origin.y + size.height * 1.5f);
                auto inAnimation = MoveTo::create(0.5f, Vec2(origin.x + size.width / 2, origin.y + size.height / 2));
                playSprite->runAction(inAnimation);
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

    auto tableHeaderRanking = Label::createWithTTF(Constants::STATS_TABLE_HEADER_RANKING, Constants::DEFAULT_FONT_BOLD_PATH, Constants::STATS_FONT_SIZE);
    tableHeaderRanking->setPosition(statsSpriteSize.width * 0.27f, statsSpriteSize.height * 0.66f);
    tableHeaderRanking->setColor(Color3B(88, 105, 132));
    statsSprite->addChild(tableHeaderRanking);

    auto tableHeaderScore = Label::createWithTTF(Constants::STATS_TABLE_HEADER_SCORE, Constants::DEFAULT_FONT_BOLD_PATH, Constants::STATS_FONT_SIZE);
    tableHeaderScore->setPosition(statsSpriteSize.width * 0.75f, statsSpriteSize.height * 0.66f);
    tableHeaderScore->setColor(Color3B(88, 105, 132));
    statsSprite->addChild(tableHeaderScore);

    for (int i = 0; i < top5Record.size(); ++i){
        auto record = Label::createWithTTF(top5Record.at(i), Constants::DEFAULT_FONT_PATH, Constants::STATS_FONT_SIZE);
        record->setPosition(statsSpriteSize.width * 0.7f, statsSpriteSize.height * (0.55f - 0.1f * i));
        record->setColor(Color3B(88, 105, 132));
        statsSprite->addChild(record);

        auto rankSprite = Sprite::createWithSpriteFrameName(Constants::RANKING_SPRITE_PATH + std::to_string(i) + ".png");
        rankSprite->setScale(0.63f);
        rankSprite->setPosition(statsSpriteSize.width * 0.27f, statsSpriteSize.height * (0.55f - 0.1f * i));
        statsSprite->addChild(rankSprite);
    }

    this->addChild(statsSprite);
}

void MenuLayer::close(Ref* sender){
    this->removeFromParentAndCleanup(true);
}
