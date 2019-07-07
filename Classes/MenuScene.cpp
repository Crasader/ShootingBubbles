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

#include "MenuScene.h"
#include "SimpleAudioEngine.h"
#include "DataManager.h"
#include "Constants.h"


USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MenuScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    size = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();

    Sprite* background = Sprite::createWithSpriteFrameName(Constants::BACKROUND_PATH);
    background->setPosition(origin.x + size.width / 2, origin.y + size.height / 2);
    this->addChild(background);

    bubble = new Bubble(this, size, origin, false, Constants::MENU_MAP_ID);

    menuLayer = MenuLayer::create();
    this->addChild(menuLayer, 10);

    //Ads
    #if defined(__ANDROID__)
    // Android ad unit IDs.
    const char* kBannerAdUnit = "ca-app-pub-1390690391227171/6569589030";
    const char* kInterstitialAdUnit = "ca-app-pub-1390690391227171/4606728535";
    #else
    // iOS ad unit IDs.
    const char* kBannerAdUnit = "ca-app-pub-1390690391227171/6569589030";
    const char* kInterstitialAdUnit = "ca-app-pub-1390690391227171/4606728535";
    #endif
    // Create and initialize banner view.
    banner_view = new firebase::admob::BannerView();
    firebase::admob::AdSize ad_size;
    ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
    ad_size.width = 320;
    ad_size.height = 50;
    banner_view->Initialize(getAdParent(), kBannerAdUnit, ad_size);

    // Create ad InterstitialAdUnit
    stillNotLoadInterstitial = false;
    interstitial_view = new firebase::admob::InterstitialAd();
    interstitial_view->Initialize(getAdParent(), kInterstitialAdUnit);

    this->scheduleUpdate();

    return true;
}

void MenuScene::update(float delta){
    //Update for bubble
    //Make bubbles goes by the wall
    bubble->update(1.0f);

    // Check that the banner has been initialized.
    if (banner_view->InitializeLastResult().status() == firebase::kFutureStatusComplete
        && banner_view->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone) {
        // Check that the banner hasn't started loading.
        if (banner_view->LoadAdLastResult().status() == firebase::kFutureStatusInvalid) {
            // Make the banner visible and load an ad.
            banner_view->Show();
            firebase::admob::AdRequest my_ad_request = {};
            banner_view->LoadAd(my_ad_request);
            banner_view->MoveTo(firebase::admob::BannerView::kPositionBottom);
        }
    }


    //Check that the interstitial has been initialized.
    if (interstitial_view->InitializeLastResult().status() == firebase::kFutureStatusComplete
        && interstitial_view->InitializeLastResult().error() == firebase::admob::kAdMobErrorNone) {
        // Check that the interstitial hasn't started loading.
        if (interstitial_view->LoadAdLastResult().status() == firebase::kFutureStatusInvalid) {
            firebase::admob::AdRequest my_ad_request = {};
            interstitial_view->LoadAd(my_ad_request);
        }
        if (interstitial_view->LoadAdLastResult().status() == firebase::kFutureStatusComplete &&
            interstitial_view->LoadAdLastResult().error() == firebase::admob::kAdMobErrorNone &&
            !stillNotLoadInterstitial){
            DataManager* dataManager = new DataManager();
            if (dataManager->init()){
                if (dataManager->countPlayingTimes() == 2){
                    // Make the interstitial visible and load an ad.
                    interstitial_view->Show();
                    stillNotLoadInterstitial = true;
                }
                dataManager->close();
            }

            delete dataManager;
        }
    }
}

void MenuScene::onExitTransitionDidStart(){
    banner_view->Destroy();
}

void MenuScene::menuCloseCallback(Ref* pSender)
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
