#pragma once

#include "cocos2d.h"

using namespace cocos2d;


class CBullet;

class CBase : public CCNode
{
public:
    CBase(void);
    ~CBase(void);
    
    bool init(CCNode *parent);
    
    bool isCollide(CBullet *bullet);

    void setActive(bool active);

    bool getActive(void);

    void setPosition(CCPoint pt);
    const CCPoint& getPosition(void);

    static CBase* createBase(CCNode *parent);



protected:
    CCSprite    *m_pBase;

    bool m_bActive;
};

