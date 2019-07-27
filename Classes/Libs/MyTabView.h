#ifndef __MY_TAB_VIEW_H__
#define __MY_TAB_VIEW_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

USING_NS_CC;

class TabView{
public:

    const static int TABS_POS_LEFT = 0;
    const static int TABS_POS_UP = 1;
    const static int TAB_SEPERATE_WIDTH = 20;
    const static int TAB_SEPERATE_HEIGHT = 20;

    TabView(Node* container);
    ~TabView();

    void setTabsPos(int pos);
    void setPosition(Vec2 pos);

    void setNumOfTabs(int value);

    void pushTab(std::string tabButtonPath0, std::string tabButtonPath1, std::string panelPath, bool useTexturePacker);

    Sprite* getPanelOfTab(int tabIndex);

    void setPanelColor(Color3B color);
private:
    Node* container;
    Vec2 position;

    int tabsPos;
    int numOfTabs;

    struct TabViewElement;
    std::vector<TabViewElement*> *tabView;

    void setTabElementPosition(TabViewElement *&element);
    void addClickEventToTab(TabViewElement*& element);

    int currentSelectedTab;

    Color3B panelColor;
};

#endif
