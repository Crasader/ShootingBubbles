#include "PlayScene.h"
#include "SimpleAudioEngine.h"

#include "../../Libs/ControlLayer.h"
#include "NormalMap.h"
#include "SpeedMap.h"
#include "WindMillMap.h"
#include "BossMap.h"

#include "../../Libs/GameLibrary.h"

USING_NS_CC;

Scene* PlayScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));

    auto layer = PlayScene::create();
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool PlayScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    ControlLayer* controlLayer = ControlLayer::create();

    int map = 0;

    DataManager* dataManager = new DataManager();
    if (dataManager->init()){
        map = dataManager->getMap();
        dataManager->close();
    }
    delete dataManager;

    switch(map){
        case 1:
        {
            SpeedMap* speedMap = SpeedMap::create();
            speedMap->setController(controlLayer);
            this->addChild(speedMap);
            break;
        }
        case 2:
        {
            WindMillMap* windMillMap = WindMillMap::create();
            windMillMap->setController(controlLayer);
            this->addChild(windMillMap);
            break;
        }
        case 3:
        {
            BossMap* bossMap = BossMap::create();
            bossMap->setController(controlLayer);
            this->addChild(bossMap);
            break;
        }
        default:
        {
            NormalMap* normalMap = NormalMap::create();
            normalMap->setController(controlLayer);
            this->addChild(normalMap);
            break;
        }
    }
    this->addChild(controlLayer);
    return true;
}

void PlayScene::menuCloseCallback(Ref* pSender)
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
