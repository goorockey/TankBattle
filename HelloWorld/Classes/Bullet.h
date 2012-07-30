#pragma once

#include "cocos2d.h"
#include "Tank.h"
using namespace cocos2d;

class CBullet : public CCNode
{
public:
    CBullet(eMoveDirection direction = MOVE_UP);
    ~CBullet();

    void setDirection(eMoveDirection direction);
    eMoveDirection getDireciton();

    void setPosition(const CCPoint& pos);
    const CCPoint& getPosition();

    virtual bool init(CCNode *parent);

    CCSprite* getBullet();

    CCSize getBulletSize();

    static CBullet* createBullet( CCNode *parent, eMoveDirection direction );
protected:
    eMoveDirection m_Direction;

    CCSprite *m_pBullet;
};