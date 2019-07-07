#include "Boss.h"

USING_NS_CC;

Boss::Boss(Node* playLayer, Size size, Vec2 origin, bool isScale){
    this->playLayer = playLayer;
    this->size = size;
    this->origin = origin;

    //Init boss sprite
    int bossColor = RandomHelper::random_int(0, Constants::BOSS_NUM_COLOR - 1);
    int bossType = RandomHelper::random_int(3, 2 + Constants::BOSS_NUM_OF_TYPES);

    bossSprite = Sprite::createWithSpriteFrameName(Constants::BOSS_PATH + std::to_string(bossType)
                            + "Shot_" + std::to_string(bossColor) + ".png");

    if (isScale){
        float scale = 0.5f + RandomHelper::random_int(0, 9) * 0.05f;
        bossSprite->setScale(scale);
    }

    PhysicsBody* physicsBody;
    switch (bossType){
        case 3:
        {
            float bossSize = bossSprite->getContentSize().width * 0.95f;
            Vec2 physicsBodyPoints[] = {
                Vec2(-bossSize * 0.5f, -bossSize * 0.5f),
                Vec2(bossSize * 0.5f, -bossSize * 0.5f),
                Vec2(0.0f, bossSize * 0.5f)
            };
            physicsBody = PhysicsBody::createPolygon(physicsBodyPoints, 3);
            break;
        }
        case 4:
        {
            float bossSize = bossSprite->getContentSize().width * 0.9f;
            Vec2 physicsBodyPoints[] = {
                Vec2(-bossSize * 0.5f, -bossSize * 0.5f),
                Vec2(bossSize * 0.5f, -bossSize * 0.5f),
                Vec2(-bossSize * 0.5f, bossSize * 0.5f),
                Vec2(bossSize * 0.5f, bossSize * 0.5f),
            };
            physicsBody = PhysicsBody::createPolygon(physicsBodyPoints, 4);
            break;
        }
        case 5:
        {
            float bossSize = bossSprite->getContentSize().width * 0.9f;
            Vec2 physicsBodyPoints[] = {
                Vec2(0, bossSize * 0.55278 - 11.0f),
                Vec2(bossSize * 0.52573f, bossSize * 0.17082f - 11.0f),
                Vec2(bossSize * 0.32491f, -bossSize * 0.44721f - 11.0f),
                Vec2(-bossSize * 0.32491f, -bossSize * 0.44721f - 11.0f),
                Vec2(-bossSize * 0.52573f, bossSize * 0.17082f - 11.0f)
            };
            physicsBody = PhysicsBody::createPolygon(physicsBodyPoints, 5);
            break;
        }
        case 6:
        {
            float bossSize = bossSprite->getContentSize().width * 0.9f;
            Vec2 physicsBodyPoints[] = {
                Vec2(0, bossSize * 0.55),
                Vec2(0, -bossSize * 0.55),
                Vec2(bossSize * 0.46f, -bossSize * 0.26f),
                Vec2(bossSize * 0.46f, bossSize *  0.26f),
                Vec2(-bossSize * 0.46f, -bossSize * 0.26f),
                Vec2(-bossSize * 0.46f, bossSize *  0.26f)
            };
            physicsBody = PhysicsBody::createPolygon(physicsBodyPoints, 6);
            break;
        }
        default:
            break;
    }

    physicsBody->setContactTestBitmask(Constants::BOSS_COLLISION_BITMASK);
    physicsBody->setCollisionBitmask(Constants::BOSS_COLLISION_BITMASK);
    physicsBody->setCategoryBitmask(Constants::BOSS_CATEGORY_BITMASK);
    bossSprite->setPhysicsBody(physicsBody);

    //Init hp bar
    MAX_HP = bossType;
    HP = MAX_HP;
    NUM_OF_BULLET_HEAD = bossType;

    hpBar = ui::LoadingBar::create(Constants::BOSS_HP_BAR, ui::Widget::TextureResType::PLIST);
    hpBar->setOpacity(Constants::BOSS_HP_BAR_OPACITY);
    hpBar->setPercent(100);

    auto hpBarContainer = Sprite::createWithSpriteFrameName(Constants::BOSS_HP_BAR_CONTAINER);
    hpBarContainer->setOpacity(Constants::BOSS_HP_BAR_CONTAINER_OPACITY);
    hpBarContainer->addChild(hpBar);

    //Init container of hp bar and boss
    container = Node::create();
    container->setContentSize(bossSprite->getContentSize());

    container->addChild(bossSprite);
    container->addChild(hpBarContainer);

    playLayer->addChild(container, Constants::BOSS_Z_ORDER);

    //Positioning
    bossSprite->setPosition(Vec2(0, 0));
    hpBar->setPosition(hpBarContainer->getContentSize() / 2);
    hpBarContainer->setPosition(Vec2(0, container->getContentSize().height * 0.65f));
    if (RandomHelper::random_int(0,1)){
        //Left
        container->setPosition(origin.x, origin.y + size.height * 0.5f);
    }
    else {
        //Right
        container->setPosition(origin.x + size.width, origin.y + size.height * 0.5f);
    }

    //Movement and rotation
    vecMovement = Vec2(RandomHelper::random_real(-Constants::BOSS_V, Constants::BOSS_V),
                            RandomHelper::random_real(-Constants::BOSS_V, Constants::BOSS_V));
    auto move = MoveBy::create(1.0f, vecMovement);
    auto actionMove = RepeatForever::create(move);
    container->runAction(actionMove);

    auto rotate = RotateBy::create(Constants::BOSS_ROTATE_SPEED, 360.0f);
    auto action = RepeatForever::create(rotate);
    bossSprite->runAction(action);

    // Other attributes

    bossBulletSet = new std::vector<Sprite*>();
    headPosition = new Vec2[NUM_OF_BULLET_HEAD];
}

Boss::~Boss(){
    while (!bossBulletSet->empty()){
        bossBulletSet->at(0)->removeFromParent();
        bossBulletSet->erase(bossBulletSet->begin());
    }
    delete[] headPosition;
    delete bossBulletSet;
    container->removeFromParent();
}

void Boss::shoot(){
    //Get heads position
    float rotation = bossSprite->getRotation();
    Vec2 center = container->getPosition();

    float ratio = bossSprite->getContentSize().width / 2;
    for (int i = 0; i < NUM_OF_BULLET_HEAD; ++i){
        float headX = center.x + ratio * sin(rotation * Constants::PI / 180);
        float headY = center.y + ratio * cos(rotation * Constants::PI / 180);
        rotation += 360 / NUM_OF_BULLET_HEAD;

        headPosition[i] = Vec2(headX, headY);
    }

    //Add sprite

    for (int i = 0; i < NUM_OF_BULLET_HEAD; ++i){
        Sprite* bullet = Sprite::createWithSpriteFrameName(Constants::BOSS_BULLET_PATH);
        bullet->setPosition(headPosition[i]);
        bullet->setScale(bossSprite->getScale());

        auto physicsBody = PhysicsBody::createCircle(bullet->getContentSize().width / 2);

        physicsBody->setContactTestBitmask(Constants::BOSS_BULLET_COLLISION_BITMASK);
        physicsBody->setCollisionBitmask(Constants::BOSS_BULLET_COLLISION_BITMASK);
        physicsBody->setCategoryBitmask(Constants::BOSS_BULLET_CATEGORY_BITMASK);
        bullet->setPhysicsBody(physicsBody);

        this->playLayer->addChild(bullet, Constants::BOSS_BULLET_Z_ORDER);
        bossBulletSet->push_back(bullet);

        float deltaX = (headPosition[i].x - center.x) * Constants::BOSS_BULLET_V;
        float deltaY = (headPosition[i].y - center.y) * Constants::BOSS_BULLET_V;

        physicsBody->setVelocity(Vec2(deltaX, deltaY));
    }
}

bool Boss::bulletIsEmpty(){
    return bossBulletSet->empty();
}

Vec2 Boss::getPosition(){
    return container->getPosition();
}

void Boss::setPosition(Point pos){
    container->setPosition(pos);
}

void Boss::changeDirectionVertically(){

    //Change vecMovement.y
    if (vecMovement.y >= 0){
        vecMovement.y = RandomHelper::random_real(-Constants::BOSS_V, -Constants::BOSS_V * 0.5f);
    }
    else {
        vecMovement.y = RandomHelper::random_real(Constants::BOSS_V * 0.5f, Constants::BOSS_V);
    }

    container->stopAllActions();

    auto move = MoveBy::create(1.0f, vecMovement);
    auto actionMove = RepeatForever::create(move);
    container->runAction(actionMove);
}

void Boss::changeDirectionHorizontally(){
    //Change vecMovement.x
    if (vecMovement.x >= 0){
        vecMovement.x = RandomHelper::random_real(-Constants::BOSS_V, -Constants::BOSS_V * 0.5f);
    }
    else {
        vecMovement.x = RandomHelper::random_real(Constants::BOSS_V * 0.5f, Constants::BOSS_V);
    }

    container->stopAllActions();

    auto move = MoveBy::create(1.0f, vecMovement);
    auto actionMove = RepeatForever::create(move);
    container->runAction(actionMove);
}

int Boss::getHP(){
    return HP;
}

void Boss::setHP(int hp){
    HP = hp;
    hpBar->setPercent(100.0f * hp / MAX_HP);
}

Sprite* Boss::getBossSprite(){
    return bossSprite;
}

std::vector<Sprite*> *Boss::getBulletSet(){
    return bossBulletSet;
}

void Boss::stop(){
    container->stopAllActions();
    bossSprite->stopAllActions();
}

void Boss::update(){
    Point position = container->getPosition();
    if (position.x < origin.x){
        container->setPosition(Point(origin.x, position.y));
        changeDirectionHorizontally();
    }

    if (position.x > origin.x + size.width){
        container->setPosition(Point(origin.x + size.width, position.y));
        changeDirectionHorizontally();
    }

    if (position.y < origin.y){
        container->setPosition(Point(position.x, origin.y));
        changeDirectionVertically();
    }

    if (position.y > origin.y + size.height){
        container->setPosition(Point(position.x, origin.y + size.height));
        changeDirectionVertically();
    }

    //Delete bullet if it's out

    for (unsigned int i = 0; i < bossBulletSet->size(); ++i){
        float x = bossBulletSet->at(i)->getPosition().x;
        float y = bossBulletSet->at(i)->getPosition().y;
        if (x < origin.x - Constants::OUT_SPACE || y < origin.y - Constants::OUT_SPACE ||
            x > origin.x + size.width + Constants::OUT_SPACE || y > origin.y + size.height + Constants::OUT_SPACE){
                auto node = bossBulletSet->at(i);
                bossBulletSet->erase(bossBulletSet->begin() + i);
                node->removeFromParent();
        }
    }
}
