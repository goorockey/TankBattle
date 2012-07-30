#pragma once

#include "GameManager.h"

using namespace cocos2d;

class CGameScene;

class CMonitor : public CCNode
{
public:
    CMonitor(CGameScene *manager);
    ~CMonitor(void);

    virtual void update(float dt);
    virtual void monitorGameOver();

protected:
    void monitorBullet();

protected:
    CGameScene    *m_pGameScene;
};

