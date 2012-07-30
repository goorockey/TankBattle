#include "Player.h"
#include "GameScene.h"
#include "Tank.h"
#include "Bullet.h"
#include "Base.h"

#define  AI_COUNT 5
#define  BASE_COMEOUT_LIMIT 3

CPlayer::CPlayer(std::string name)
    : m_szPlayerName(name)
    , m_bIsLost(false)
    , m_pBase(NULL)
    , m_nDestroyedTankCnt(0)
{
    m_Tanklist = new CCArray;
}

CPlayer::~CPlayer()
{
    m_Tanklist->removeAllObjects();
    delete m_Tanklist;

    delete m_pBase;
}

bool CPlayer::init(CCNode *parent, int playerIndex)
{
    bool bRet = false;

    do 
    {
        CCSize tileSize = CGameScene::getTileSize();


        CTank *mainTank;

        m_nIndex = playerIndex;

        if (playerIndex == 1)
        {
            mainTank = CTank::createPlayerTank(this);
            for (int i = 0; i < AI_COUNT; i++)
            {
                CTank *tank = CTank::createAITank(this);
                if (tank)
                {

                    tank->setPosition(mainTank->getPosition());
                    m_Tanklist->addObject(tank);
                }
            }
        }
        else
        {
            mainTank = CTank::createPlayer2Tank(this);
            for (int i = 0; i < AI_COUNT; i++)
            {
                CTank *tank = CTank::createAITank2(this);
                if (tank)
                {
                    tank->setPosition(mainTank->getPosition());
                    m_Tanklist->addObject(tank);
                }
            }
       }

        if (mainTank)
        {
            m_Tanklist->addObject(mainTank);
        }



        CCSize mapSize = CGameScene::getMapSize();
        CCPoint basePos = mainTank->getPosition();
        if (playerIndex == 1)
        {
            basePos = ccp(mapSize.width/2 / tileSize.height * tileSize.height, mapSize.height/4 / tileSize.height * tileSize.height);
        }
        else
        {
            basePos = ccp(mapSize.width/2 / tileSize.height * tileSize.height, mapSize.height * 3/4 / tileSize.height * tileSize.height);
        }
        CBase *base = CBase::createBase(this);
        if (base)
        {
            m_pBase = base;
            m_pBase->setActive(false);
            m_pBase->setPosition(basePos);
        }

        parent->addChild(this);

        bRet = true;
    } while (0);

    return bRet;
}

cocos2d::CCArray* CPlayer::getTankList()
{
    return m_Tanklist;
}

CCArray* CPlayer::getAllBullet()
{
    CCArray *pRet = new CCArray;
    pRet->autorelease();

    CCObject *pTank;
    CCARRAY_FOREACH(m_Tanklist, pTank)
    {
        if (CTank *tank = dynamic_cast<CTank*>(pTank))
        {
            CCArray *bulletList = tank->getBulletList();

            CCObject *pBullet;
            CCARRAY_FOREACH(bulletList, pBullet)
            {
                if (CBullet *bullet = dynamic_cast<CBullet*>(pBullet))
                {
                    pRet->addObject(bullet);
                }
            }
        }
    }

    return pRet;
}

bool CPlayer::isCollide( CBullet *bullet )
{
    bool bRet = false;

    do 
    {
        if (m_pBase->getActive() == true && m_pBase->isCollide(bullet) == true)
        {
            bRet = true;
            m_bIsLost = true;
            break;
        }

        CCObject *pObj;
        CCARRAY_FOREACH(m_Tanklist, pObj)
        {
            if (CTank *tank = dynamic_cast<CTank *>(pObj))
            {
                if (tank->isCollide(bullet) == true)
                {
                    bRet = true;
                }
            }
        }

    } while (0);

//     if (m_Tanklist->count() == 0)
//     {
//         m_bIsLost = true;
//     }

    return bRet;
}


bool CPlayer::removeBullet( CBullet *bullet )
{
    CCObject *pObj;
    CCARRAY_FOREACH(m_Tanklist, pObj)
    {
        if (CTank *tank = dynamic_cast<CTank*>(pObj))
        {
            tank->removeBullet(bullet);
        }
    }
    return true;
}

CTank* CPlayer::getAnotherTank( CTank *curTank )
{
    CTank *retTank = NULL;

    if (m_Tanklist->count() >= 2)
    {
        m_Tanklist->removeObject(curTank);

        CCObject *pObj;
        CCARRAY_FOREACH(m_Tanklist, pObj)
        {
            if (CTank *tank = dynamic_cast<CTank*>(pObj))
            {
                if (tank != curTank)
                {
                    retTank = tank;
                    break;
                }
            }
        }

        m_Tanklist->addObject(curTank);
    }

    return retTank;
}



void CPlayer::removeTank( CTank *tank )
{
    if (tank && m_Tanklist->containsObject(tank))
    {
        m_Tanklist->removeObject(tank);
        this->removeChild(tank, true);

        if (++m_nDestroyedTankCnt == BASE_COMEOUT_LIMIT)
        {
            //m_pBase->setActive(true);
            displayBase();
        }
    }
}

void CPlayer::displayBase(void)
{
    m_pBase->setActive(true);


    CCSize tileSize = CGameScene::getTileSize();
    float tileWidth = tileSize.height;
    float tileHeight = tileSize.width;

    CCPoint basePos = m_pBase->getPosition();
    basePos.x -= tileWidth / 2;
    basePos.y -= tileHeight / 2;

    float offset[4][4] = {
        {0, -tileHeight, 1, 0},
        {2*tileWidth, 0, 0, 1},
        {tileWidth, 2*tileHeight, -1, 0},
        {-tileWidth, tileHeight, 0, -1}};

    for (int i = 0; i < 4; i++)
    {
        CCPoint tmp = ccp(basePos.x + offset[i][0], basePos.y + offset[i][1]);
        for (int j = 0; j < 3; j++)
        {
            CCPoint pt = tmp;
            pt.x += offset[i][2] * tileWidth * j;
            pt.y += offset[i][3] * tileHeight * j;
            
            bool ret = CGameScene::setWall(pt);
            CCLOG("%f, %f, %d", pt.x, pt.y, ret );
        }

    }
}

