#pragma once
#include "cocos2d.h"

using namespace cocos2d;

class CGameStart : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(CGameStart);

    static CCScene* scene();
    void menuPlayCallback(CCObject* pSender);
};
