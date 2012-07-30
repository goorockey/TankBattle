#pragma once

#include <string>
#include "cocos2d.h"
#include "common.h"

using namespace cocos2d;

class CBullet;
class CTank;
class CBase;

class CPlayer : public CCNode
{
    INIT_SIMPLE_PROPERTY(unsigned int, m_nDestroyedTankCnt, DetroyedTankCnt);
    INIT_SIMPLE_PROPERTY(bool, m_bIsLost, IsLost);
    INIT_SIMPLE_PROPERTY(std::string, m_szPlayerName, PlayerName);

public:
    CPlayer(std::string name);
    ~CPlayer();

    virtual bool init(CCNode *parent, int playerIndex = 1);
    
    bool isCollide( CBullet *bullet );

    bool removeBullet( CBullet *bullet );
    void removeTank(CTank *tank);

    CTank* getAnotherTank(CTank *curTank);

    CCArray* getTankList();
    CCArray* getAllBullet();

    void displayBase(void);

    int     m_nIndex;
protected:
    CCArray   *m_Tanklist;
    CBase   *m_pBase;

};