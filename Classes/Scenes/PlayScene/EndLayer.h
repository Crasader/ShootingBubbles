#ifndef __END_LAYER_H__
#define __END_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class EndLayer : public Layer

{
public:
    EndLayer();
    ~EndLayer();

    bool init() override;

    CREATE_FUNC(EndLayer);

    void close(Ref* sender=nullptr);

    void setLabel(int score);

private:
    Label* scoreLabel;
    Label* highScoreLabel;
};

#endif
