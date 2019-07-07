#include "MyTabView.h"

struct TabView::TabViewElement{
    ui::CheckBox* tab;
    Sprite* panel;
    int index;
};

TabView::TabView(Node* container){
    this->container = container;
    tabView = new std::vector<TabViewElement*>();

    tabsPos = TABS_POS_UP;
    position = Vec2(0, 0);

    currentSelectedTab = 0;
}

TabView::~TabView(){
    delete tabView;
}

void TabView::setTabsPos(int pos){
    tabsPos = pos;
}

void TabView::setPosition(Vec2 pos){
    position = pos;
}

void TabView::pushTab(std::string tabButtonPath0, std::string tabButtonPath1, std::string panelPath, bool useTexturePacker){
    TabViewElement* element = new TabViewElement();
    if (useTexturePacker){
        element->tab = ui::CheckBox::create(tabButtonPath0,
                                            tabButtonPath0,
                                            tabButtonPath1, 
                                            tabButtonPath1,
                                            tabButtonPath1,
                                            ui::Widget::TextureResType::PLIST);
        element->panel = Sprite::createWithSpriteFrameName(panelPath);
    }
    else {
        element->tab = ui::CheckBox::create(tabButtonPath0, tabButtonPath1);
        element->panel = Sprite::create(panelPath);
    }
    element->panel->setColor(panelColor);
    element->panel->setVisible(false);
    element->index = tabView->size();

    if (element->index == numOfTabs - 1){
        element->tab->setSelected(true);
        element->panel->setVisible(true);
        currentSelectedTab = element->index;
    }

    element->tab->setAnchorPoint(Point(0.5f, 0.5f));
    element->panel->setAnchorPoint(Point(0.5f, 0.5f));

    tabView->push_back(element);
    setTabElementPosition(element);
    addClickEventToTab(element);

    container->addChild(element->tab);
    container->addChild(element->panel);
}

void TabView::setTabElementPosition(TabViewElement *&element){
    element->panel->setPosition(position);

    Size tabSize = element->tab->getVirtualRendererSize();
    Size panelSize = element->panel->getContentSize();

    switch (tabsPos){

        case TABS_POS_UP:{
            element->tab->setPosition(Vec2(
                position.x + (tabSize.width + TAB_SEPERATE_WIDTH) * ((1 - numOfTabs) * 0.5f + element->index),
                position.y + panelSize.height / 2 + tabSize.height / 2
            ));
            break;
        }

        case TABS_POS_LEFT:{
            element->tab->setPosition(Vec2(
                position.x - panelSize.width / 2 - tabSize.width / 2,
                position.y + (tabSize.height + TAB_SEPERATE_HEIGHT) * ((1-numOfTabs) * 0.5f + element->index)
            ));
            break;
        }

        default: break;
    }
}

void TabView::addClickEventToTab(TabViewElement*& element){
    element->tab->addEventListener([&, element](Ref* sender, ui::CheckBox::EventType type){
        switch (type) {
            case ui::CheckBox::EventType::SELECTED: {

                tabView->at(currentSelectedTab)->panel->setVisible(false);
                tabView->at(currentSelectedTab)->tab->setSelected(false);

                element->tab->setSelected(true);
                element->panel->setVisible(true);

                currentSelectedTab = element->index;
                break;
            }
            case ui::CheckBox::EventType::UNSELECTED: {
                if (element->index == currentSelectedTab){
                    element->tab->setSelected(true);
                }
                break;
            }
            default:
            break;
        }
    });
}

void TabView::setNumOfTabs(int value){
    this->numOfTabs = value;
}

Sprite* TabView::getPanelOfTab(int tabIndex){
    return tabView->at(tabIndex)->panel;
}

void TabView::setPanelColor(Color3B color){
    panelColor = color;
}
