#pragma once

#include <vector>

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "common.h"



using namespace cocos2d;

class CController;
class CBullet;

class CTank : public CCNode
{
#define DEFAULT_TANK_MOVE_SPEED (480 / 8)
#define DEFAULT_TANK_BULLET_SPEED   (480 / 4)
#define DEFAULT_TANK_SCALE  2

public:
    static CTank* createPlayerTank(CCNode *parent);

    static CTank* createAITank(CCNode *parent);
        static CTank* createAITank2(CCNode *parent);

    static CTank* createPlayer2Tank( CCNode *parent );

public:
    CTank(float moveSpeed = DEFAULT_TANK_MOVE_SPEED, 
        float bulletSpeed = DEFAULT_TANK_BULLET_SPEED);
    ~CTank(void);

    //virtual bool init(CCNode *parent);
    bool init(CCNode *parent, char szTankPlist[], char szTankPng[]);
    bool move( eMoveDirection direction);
    void shoot();

    void setActive(bool active);
    bool getActive(void);

    void bulletMoveFinished(CCNode* sender);

    void removeBullet(CBullet *bullet);

    void setController(CController *controller);
    CController* getController(void);

    void setPosition(const CCPoint &pos);
    const CCPoint& getPosition();

    eMoveDirection getDirection();

    CCSprite* getTank();

    CCArray* getBulletList();

    bool isCollide( CBullet *bullet );
    bool isMoving();

    bool changeControllTank(void);
    void tankMoveFinished(CCNode* sender);
    void changeTankImage(eMoveDirection direction);
protected:
    CCSprite *m_pTank;
    CController *m_pController;
    CCArray     *m_BulletList;

    bool    m_bActive;
    eMoveDirection  m_TankDirection;
    float   m_fMoveSpeed;
    float   m_fBulletSpeed;
    bool    m_bMoving;
};

