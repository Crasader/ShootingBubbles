#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

#include <vector>

USING_NS_CC;

class MenuLayer : public Layer

{
public:
    MenuLayer();
    ~MenuLayer();

    bool init() override;

    CREATE_FUNC(MenuLayer);

    void close(Ref* sender=nullptr);

private:
    Size size;
    Vec2 origin;

    Sprite *playSprite, *settingSprite, *statsSprite;
    std::vector <std::string> top5Record;

    void initPlaySprite();
    void initPlayButton(Size containerSize, ui::Button*& button);
    void initSettingButton(Size containerSize, ui::Button*& button);
    void initStatsButton(Size containerSize, ui::Button*& button);

    void initSettingSprite();
    void initStatsSprite();

    void initCharacterSkinTab(Node* container);
    void initMapTab(Node* container);
    void initMusicTab(Node* container);
    void initInfoTab(Node* container);
};

#endif
