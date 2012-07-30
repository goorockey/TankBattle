#include "Tank.h"
#include "GameManager.h"
#include "GameScene.h"
#include "KeyboardController.h"
#include "AIController.h"
#include "Bullet.h"
#include "Player.h"


//const char szTankPlist[] = "material/tank2/tank.plist"; 
//const char szTankPng[] = "material/tank2/tank.png";  



CTank::CTank( float moveSpeed /*= DEFAULT_TANK_MOVE_SPEED*/, 
    float bulletSpeed /*= DEFAULT_TANK_BULLET_SPEED*/ )
    : m_fMoveSpeed(moveSpeed)
    , m_fBulletSpeed(bulletSpeed)
    , m_pController(NULL)
    , m_bMoving(false)
{
    m_BulletList = new CCArray;
}


CTank::~CTank(void)
{
//     m_BulletList->removeAllObjects();
//     delete m_BulletList;
}



bool CTank::init(CCNode *parent, char szTankPlist[], char szTankPng[])
{
    bool bRet = true;

    do
    {
        CCSpriteFrameCache* cache = CGameManager::getGameManager()->getFrameCache();

        CC_BREAK_IF(!cache);

        cache->addSpriteFramesWithFile(szTankPlist, szTankPng);  

        CCSize tileSize = CGameScene::getTileSize();
        CCSprite *tank = CCSprite::spriteWithFile("material/tank2/tank_up.png",
            CCRectMake(0,0,tileSize.width*2, tileSize.height*2));

        //tank->setScale(DEFAULT_TANK_SCALE);
        CC_BREAK_IF(!tank);
        m_pTank = tank;

        tank->setScale(2);
        tank->setPosition(ccp(tileSize.width,tileSize.height));

        this->addChild(tank);
        parent->addChild(this);

        m_TankDirection = MOVE_UP;

        changeTankImage(MOVE_UP);


        setActive(true);


        bRet = true;

    }while(0);

    return bRet;
}

void CTank::changeTankImage(eMoveDirection direction)
{
    CCSpriteFrameCache* cache = CGameManager::getGameManager()->getFrameCache();
    
    CPlayer *player = (CPlayer*)getParent();
    int index = player->m_nIndex;
    if (index == 1)
    {
    switch (direction)
    {
        case MOVE_UP:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank_up.png"));
            break;
        case MOVE_DOWN:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank_down.png"));
            break;
        case MOVE_LEFT:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank_left.png"));
            break;
        case MOVE_RIGHT:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank_right.png"));
            break;
        default:
            break;
        }
    }
    else
    {

        switch (direction)
        {
        case MOVE_UP:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank2_up.png"));
            break;
        case MOVE_DOWN:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank2_down.png"));
            break;
        case MOVE_LEFT:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank2_left.png"));
            break;
        case MOVE_RIGHT:
            m_pTank->setDisplayFrame(cache->spriteFrameByName("tank2_right.png"));
            break;
        default:
            break;
        }
    }

    CCSize tileSize = CGameScene::getTileSize();
    m_pTank->setContentSize(CCSizeMake(tileSize.width*2, tileSize.height*2));


}

bool CTank::move( eMoveDirection direction)
{
    CCAssert(direction >= MOVE_LEFT && direction <= MOVE_DOWN, "Invalid Direction");

    m_TankDirection = direction;


    // 改变坦克图像
    changeTankImage(direction);

    // 计算判断是否能往前移动
    CCSize tileSize = CGameScene::getTileSize();
    CCPoint TargetPos = m_pTank->getPosition();

    bool bIsMove = CGameScene::canMoveForward(direction, tileSize.height, 
        m_pTank->getContentSize(), TargetPos);
 
    if (bIsMove)
    {
        //CCLOG("Before Move: %f, %f", m_pTank->getPosition().x, m_pTank->getPosition().y);
        float duration = CGameScene::calcDistance(m_pTank->getPosition(), TargetPos) 
            / m_fMoveSpeed;

        // 移动坦克
        CCFiniteTimeAction* actionMove = CCMoveTo::actionWithDuration(duration, TargetPos);
        CCFiniteTimeAction* actionMoveDone = CCCallFuncN::actionWithTarget( this, callfuncN_selector(CTank::tankMoveFinished));

        m_pTank->runAction( CCSequence::actions(actionMove, actionMoveDone, NULL) );

        m_bMoving = true;
    }

    return bIsMove;
}

void CTank::shoot()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize tileSize = CGameScene::getTileSize();

    CCPoint tankPos = getPosition();
    CCSize tankSize = m_pTank->getContentSize();


    CCPoint bulletPos = tankPos;
    CCPoint bulletTarget = bulletPos;

    CBullet *bullet = CBullet::createBullet(this, m_TankDirection);

    // 设置子弹初始位置
    switch(m_TankDirection)
    {
    case MOVE_UP:
        bulletPos.y += tankSize.height/2;
        bulletTarget.y = winSize.height - bullet->getContentSize().height/2;
        break;
    case MOVE_DOWN:
        bulletPos.y -= tankSize.height/2;
        bulletTarget.y = bullet->getContentSize().height/2;
        break;
    case MOVE_LEFT:
        bulletPos.x -= tankSize.width/2;
        bulletTarget.x = bullet->getContentSize().width/2;
        break;
    case MOVE_RIGHT:
        bulletPos.x += tankSize.width/2;
        bulletTarget.x = winSize.width - bullet->getContentSize().width/2;
        break;
    default:
        CCAssert(0, "Invalid direction");
        break;
    }

    bulletPos = CGameScene::correctPoint(bulletPos);

    bullet->setPosition(bulletPos);
    m_BulletList->addObject(bullet);


    float duration = CGameScene::calcDistance(bulletPos, bulletTarget) / m_fBulletSpeed;

    bullet->getBullet()->runAction(CCSequence::actions(
        CCMoveTo::actionWithDuration(duration, bulletTarget),
        CCCallFuncN::actionWithTarget(this, callfuncN_selector(CTank::bulletMoveFinished)),
        NULL
        ));

    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("material/laser.wav");
}


void CTank::setActive( bool active )
{
    m_bActive = active;

    if (m_pTank)
    {
        m_pTank->setVisible(active);
    }
    
    if (m_pController)
    {
        if (m_bActive)
        {
            m_pController->scheduleUpdate();
        }
        else
        {
            m_pController->unscheduleUpdate();
        }
    }

    CCObject *bullet;
    CCARRAY_FOREACH(m_BulletList, bullet)
    {
        bulletMoveFinished((CCNode*)bullet);
    }
}

bool CTank::getActive( void )
{
    return m_bActive;
}

void CTank::removeBullet( CBullet *bullet )
{
    if (bullet && m_BulletList->containsObject(bullet))
    {
        this->removeChild(bullet, true);
        m_BulletList->removeObject(bullet);
    }
}

void CTank::tankMoveFinished(CCNode* sender)
{
    CCSprite *tank = (CCSprite *)sender;
    //CCLOG("AFter Move: %f, %f", m_pTank->getPosition().x, m_pTank->getPosition().y);
    m_bMoving = false;
}

void CTank::bulletMoveFinished(CCNode* sender)
{
    CCSprite *bullet = (CCSprite *)sender;
    removeBullet(dynamic_cast<CBullet*>(bullet->getParent()));
}

void CTank::setController( CController *controller )
{
    m_pController = controller;    

    m_pController->setTank(this);

    if (getActive())
    {
        m_pController->unscheduleUpdate();
        m_pController->scheduleUpdate();
    }
    else
    {
        m_pController->unscheduleUpdate();
    }
}

void CTank::setPosition( const CCPoint &pos )
{
    m_pTank->setPosition(pos);
}

const CCPoint& CTank::getPosition()
{
    return m_pTank->getPosition();
}

CCArray* CTank::getBulletList()
{
    return m_BulletList;
}


CTank* CTank::createPlayerTank( CCNode *parent )
{
    CTank *tank = new CTank;
    if (tank && tank->init(parent,"material/tanks/tank1.plist", "material/tanks/tank1.png"))
    {
        tank->autorelease();

        CKeyboardController *controller = new CKeyboardController(tank);
        if (controller && controller->init(tank))
        {
            return tank;
        }

    }

    CC_SAFE_RELEASE(tank);
    return NULL;
}

CTank* CTank::createPlayer2Tank( CCNode *parent )
{
    CTank *tank = new CTank;
    if (tank && tank->init(parent, "material/tanks/tank2.plist", "material/tanks/tank2.png"))
    {
        CCSize tileSize = CGameScene::getTileSize();
        CCSize mapSize = CGameScene::getMapSize();
        tank->setPosition(ccp(mapSize.width - tileSize.width, mapSize.height - tileSize.height));

        tank->autorelease();

        CKeyboardController2 *controller = new CKeyboardController2(tank);
        if (controller && controller->init(tank))
        {
            return tank;
        }

    }

    CC_SAFE_RELEASE(tank);
    return NULL;
}
CTank* CTank::createAITank2( CCNode *parent )
{
    CTank *tank = new CTank;
    if (tank && tank->init(parent, "material/tanks/tank2.plist", "material/tanks/tank2.png"))
    {
        tank->autorelease();

        CAIController *controller = new CAIController(tank);
        if(controller && controller->init(tank))
        {
            return tank;
        }
    }


    CC_SAFE_RELEASE(tank);
    return NULL;

}
CTank* CTank::createAITank( CCNode *parent )
{
    CTank *tank = new CTank;
    if (tank && tank->init(parent,"material/tanks/tank1.plist", "material/tanks/tank1.png"))
    {
        tank->autorelease();

        CAIController *controller = new CAIController(tank);
        if(controller && controller->init(tank))
        {
            return tank;
        }
    }
    

    CC_SAFE_RELEASE(tank);
    return NULL;
    
}

CCSprite* CTank::getTank()
{
    return m_pTank;
}

bool CTank::isCollide( CBullet *bullet )
{
    bool bRet = false;

    if (m_BulletList->containsObject(bullet) == false)
    {
        CCRect tankRect = CGameScene::calcSpriteRect(m_pTank);
        CCRect bulletRect = CGameScene::calcSpriteRect(bullet->getBullet());

        if (CCRect::CCRectIntersectsRect(tankRect, bulletRect) == true)
        {
            if (dynamic_cast<CKeyboardController*>(m_pController)
                || dynamic_cast<CKeyboardController2*>(m_pController))
            {
                changeControllTank();
            }

            setActive(false);

            if (CPlayer *player = dynamic_cast<CPlayer*>(getParent()))
            {
                player->removeTank(this);
            }
            bRet = true;
        }
        else
        {
            CCObject *pBullet;
            CCARRAY_FOREACH(m_BulletList, pBullet)
            {
                if (CBullet *bullet2 = dynamic_cast<CBullet*>(pBullet))
                {
                    CCRect bulletRect2 = CGameScene::calcSpriteRect(bullet2->getBullet());
                    if (CCRect::CCRectIntersectsRect(bulletRect, bulletRect2))
                    {
                        removeBullet(bullet2);
                        bRet = true;
                    }
                }
            }
        }    
    }

    return bRet;
}

bool CTank::changeControllTank( void )
{
    CController *myController = m_pController;

    if (CPlayer *player = dynamic_cast<CPlayer*>(getParent()))
    {
        CTank *otherTank = player->getAnotherTank(this);
        if (otherTank)
        {
            CController *otherController = otherTank->getController();
            if (otherController)
            {
                this->setController(otherController);
                otherTank->setController(myController);
                return true;
            }
        }
    }

    return false;
}

CController* CTank::getController( void )
{
    return m_pController;
}

bool CTank::isMoving()
{
    return m_bMoving;
}

eMoveDirection CTank::getDirection()
{
    return m_TankDirection;
}


