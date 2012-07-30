#include "Monitor.h"
#include "GameScene.h"
#include "Player.h"
#include "Bullet.h"

CMonitor::CMonitor( CGameScene *gameScene )
    : m_pGameScene(gameScene)
{

}


CMonitor::~CMonitor(void)
{
}

void CMonitor::monitorBullet()
{
    CCArray *playerList = m_pGameScene->getPlayerList();

    if (playerList)
    {
        CCObject *pPlayer = NULL;
        CCARRAY_FOREACH(playerList, pPlayer)
        {
            if (CPlayer *player = dynamic_cast<CPlayer*>(pPlayer))
            {
                CCArray *bulletList = player->getAllBullet();
                CCObject *pBullet;
                CCARRAY_FOREACH(bulletList, pBullet)
                {
                    if (CBullet *bullet = dynamic_cast<CBullet*>(pBullet))
                    {
                        // 遍历其他player的坦克
                        CCObject *pOtherPlayer;
                        CCARRAY_FOREACH(playerList, pOtherPlayer)
                        {
                            if (CPlayer *otherPlayer = dynamic_cast<CPlayer*>(pOtherPlayer))
                            {
                                if (otherPlayer != player)
                                {
                                    if (otherPlayer->isCollide(bullet) == true)
                                    {
                                        player->removeBullet(bullet);
                                        break;
                                    }
                                }
                            }
                        }


                        // 遍历地图
                        bool bCollideWall = false;
                        CCSize tileSize = CGameScene::getTileSize();
                        CCPoint bulletPos = bullet->getPosition();
                        CCPoint wallPos1 = bulletPos, wallPos2 = bulletPos;
                        switch (bullet->getDireciton())
                        {
                        case MOVE_UP:
                        case MOVE_DOWN:
                            wallPos1.x -= tileSize.width/2;
                            wallPos2.x += tileSize.width/2;
                            break;
                        case MOVE_LEFT:
                        case MOVE_RIGHT:
                            wallPos1.y -= tileSize.height/2;
                            wallPos2.y += tileSize.height/2;
                            break;
                        default:
                            break;
                        }

                        if (CGameScene::canPassThough(wallPos1, true) == false)
                        {
                            bCollideWall = true;
                        }
                        if (CGameScene::canPassThough(wallPos2, true) == false)
                        {
                            bCollideWall = true;
                        }


                        //bool bMoveForward = CGameScene::canMoveForward(bullet->getDireciton(), 0, bullet->getBulletSize(), bulletPos, true);

                        if (bCollideWall == true)
                        {
                            player->removeBullet(bullet);
                        }
                    }
                }
            }
        }
    }

}

void CMonitor::update( float dt )
{
    monitorBullet();
    monitorGameOver();
}

void CMonitor::monitorGameOver()
{
    CCArray *playerList = m_pGameScene->getPlayerList();

    unsigned int loserCnt = 0;
    CCObject *pObj;
    CCARRAY_FOREACH(playerList, pObj)
    {
        if (CPlayer *player = dynamic_cast<CPlayer*>(pObj))
        {
            if (player->getIsLost() == true)
            {
                loserCnt++;
            }
        }
    }

    if (loserCnt >= playerList->count() - 1)
    {
        CGameScene::gameOver();
    }
}
