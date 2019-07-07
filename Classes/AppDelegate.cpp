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

#include "AppDelegate.h"
#include "Constants.h"
#include "HelloWorldScene.h"
#include "DataManager.h"

#include "base/ZipUtils.h"

#include "firebase/admob.h"
#include "firebase/app.h"

// #define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size smallResolutionSize = cocos2d::Size(640, 360);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1366, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1080);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Shooting Bubbles", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("ShootingBubbles");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of small size.
    else
    {
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

    //AdMob
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    firebase::App* app = firebase::App::Create(firebase::AppOptions(), JniHelper::getEnv(), JniHelper::getActivity());
    firebase::admob::Initialize(*app, "ca-app-pub-1390690391227171~2042599789");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    firebase::App* app = firebase::App::Create(firebase::AppOptions());
        firebase::admob::Initialize(*app, "INSERT_YOUR_ADMOB_IOS_APP_ID");
#endif

    //Set playing time to 0
    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        dataManager->setCountPlayingTimes(0);
        dataManager->close();
    }

    delete dataManager;

    //Preloads background music and audio
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

    audio->setEffectsVolume(Constants::AUDIO_VOLUME);
    audio->preloadEffect(Constants::SHOOTING_AUDIO.c_str());
    audio->preloadEffect(Constants::EXPLOSION_AUDIO.c_str());
    audio->preloadEffect(Constants::DIE_AUDIO.c_str());
    audio->preloadEffect(Constants::BOSS_SHOOTING_AUDIO.c_str());
    audio->preloadEffect(Constants::ITEM_PICKUP_AUDIO.c_str());
    audio->preloadEffect(Constants::BUTTON_CLICK_AUDIO.c_str());

    audio->preloadBackgroundMusic(Constants::BACKGROUND_MUSIC.c_str());

    //Texture Atlas
    ZipUtils::setPvrEncryptionKey(0x0bc352a2, 0x1fcafa84, 0xfb06dbaf, 0x44869a85);

    for (int i = 0; i < Constants::NUM_OF_TEXTURE_PACKER_FILES; i++){
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("gameComponents-" + std::to_string(i) + ".plist");
    }

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
