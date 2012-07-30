#include "GameManager.h"
#include "GameStart.h"
#include "GameScene.h"
#include "GameOver.h"
#include "SimpleAudioEngine.h"

#include "Monitor.h"
#include "Player.h"
#include "Tank.h"


USING_NS_CC;

CCTMXTiledMap* CGameScene::m_pGameMap = NULL;

const char szMapLayer[] = "map";

CGameScene::CGameScene()
    : m_pMonitor(NULL)
{
    m_PlayerList = new CCArray;
}


CGameScene::~CGameScene()
{
    m_PlayerList->removeAllObjects();
    delete m_PlayerList;
}


CCScene* CGameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CGameScene *layer = CGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool CGameScene::init()
{
    bool bRet = false;

    do 
    {    
        if ( !CCLayer::init() )
        {
            return false;
        }


        startNewGame();



        bRet = true;
    } while (0);


    return bRet;
}


CCPoint CGameScene::tileCoordForPosition(CCPoint &position)
{
    CCSize mapSize = getMapSize();
    if (position.x < 0)
    {
        position.x = 0;
    }
    if (position.x > mapSize.width)
    {
        position.x = mapSize.width;
    }
    if (position.y < 0)
    {
        position.y = 0;
    }
    if (position.y > mapSize.height)
    {
        position.y = mapSize.height;
    }

    int x = int(position.x / m_pGameMap->getTileSize().width);
    int y = int((mapSize.height - position.y) / m_pGameMap->getTileSize().height);
    return ccp(x, y);
}

bool CGameScene::setWall(CCPoint &point)
{
    CCTMXLayer *wallLayer = m_pGameMap->layerNamed(szMapLayer);
    if (wallLayer)
    {
        if (point.x >= m_pGameMap->getMapSize().width* m_pGameMap->getTileSize().width
            || point.x <= 0
            || point.y >= m_pGameMap->getMapSize().height* m_pGameMap->getTileSize().height
            || point.y <= 0)
        {
            return false;
        }

        CCPoint tmpPt = tileCoordForPosition(point);
        unsigned int gid = wallLayer->tileGIDAt(tmpPt);
        if (gid > 0)
        {
            if (CCDictionary *dictinary = m_pGameMap->propertiesForGID(gid))
            {
                const CCString *name = dictinary->valueForKey("name");
                if (name && name->compare("steel") != 0)
                {
                    wallLayer->setTileGID(3, tmpPt);
                    return true;
                }
            }
        }
    }

    return false;
}

bool CGameScene::canPassThough(CCPoint point, bool bUpdate)
{
    CCTMXLayer *wallLayer = m_pGameMap->layerNamed(szMapLayer);
    if (wallLayer)
    {
        if (point.x >= m_pGameMap->getMapSize().width* m_pGameMap->getTileSize().width
            || point.x <= 0
            || point.y >= m_pGameMap->getMapSize().height* m_pGameMap->getTileSize().height
            || point.y <= 0)
        {
            return false;
        }

        CCPoint tmpPt = tileCoordForPosition(point);
        unsigned int gid = wallLayer->tileGIDAt(tmpPt);
        if (gid > 0)
        {
            if (CCDictionary *dictinary = m_pGameMap->propertiesForGID(gid))
            {
                const CCString *name = dictinary->valueForKey("name");
                if (name 
                    && (name->compare("wall") == 0 || name->compare("steel") == 0))
                {
                    if (bUpdate && name->compare("wall") == 0)
                    {
                        wallLayer->removeTileAt(tmpPt);
                    }
                    return false;
                }
            }
        }
    }
    
    return true;
}


void CGameScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void CGameScene::menuNewGameCallback(CCObject* pSender)
{
    startNewGame();
}





cocos2d::CCSize CGameScene::getMapSize()
{
    CCSize size;
    if (m_pGameMap)
    {
        size.height = m_pGameMap->getMapSize().height * m_pGameMap->getTileSize().height;
        size.width = m_pGameMap->getMapSize().width * m_pGameMap->getTileSize().width;
    }

    return size;
}

float CGameScene::calcDistance( const CCPoint &pt1, const CCPoint &pt2 )
{
    float distanceX = pt1.x - pt2.x;
    float distanceY = pt1.y - pt2.y;

    float distance = sqrtf(distanceX * distanceX + distanceY * distanceY);
    
    //CCAssert(int(distance) % 12 == 0);
    return distance;
}

cocos2d::CCSize CGameScene::getTileSize()
{
    return m_pGameMap->getTileSize();
}



cocos2d::CCArray* CGameScene::getPlayerList()
{
    return m_PlayerList;
}

cocos2d::CCRect CGameScene::calcSpriteRect(CCSprite *sprite )
{
    const CCPoint spritePos = sprite->getPosition();
    CCSize spriteSize = sprite->getContentSize();

    CCRect spriteRect = CCRectMake(spritePos.x - spriteSize.width/2,
        spritePos.y - spriteSize.height/2,
        spriteSize.width, spriteSize.height);

    return spriteRect;
}

void CGameScene::gameOver()
{
    CGameManager::getGameManager()->runSceneWithId(CGameManager::SCENE_GAMEOVER);
}

void CGameScene::createMenuButton()
{
    CCMenuItemImage *pNewGameItem = CCMenuItemImage::create(
        "new-game-button.png",
        "new-game-button-over.png",
        this,
        menu_selector(CGameScene::menuNewGameCallback) );

    CCSize buttonSize = pNewGameItem->getContentSize();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    pNewGameItem->setPosition( ccp(winSize.width - buttonSize.width/2 - 2, 50 + buttonSize.height/2) );

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        this,
        menu_selector(CGameScene::menuCloseCallback) );
    pCloseItem->setPosition( ccp(winSize.width - 20, 20) );

    CCMenu* pMenu = CCMenu::create(pNewGameItem, pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
}

bool CGameScene::createPlayer()
{
    bool bRet = false;

    do 
    {    
        // player1
        CPlayer *player1 = new CPlayer("Player1");
        CC_BREAK_IF(!player1 || !player1->init(this));
        m_PlayerList->addObject(player1);


        // player2
        CPlayer *player2 = new CPlayer("Player2");
        CC_BREAK_IF(!player2 || !player2->init(this, 2));
        m_PlayerList->addObject(player2);

//         CCSize mapSize = getMapSize();
//         CCArray *tankList = player2->getTankList();
//         CCObject *pObj;
//         CCARRAY_FOREACH(tankList, pObj)
//         {
//             if (CTank *tank = dynamic_cast<CTank*>(pObj))
//             {
//                 tank->setPosition(ccp(mapSize.width - 32, mapSize.height-32));
//             }
//         }


        bRet = true;
    } while (0);

    return bRet;

}

bool CGameScene::startNewGame()
{
    bool bRet = false;

    do 
    {
        this->removeAllChildrenWithCleanup(true);
        m_PlayerList->removeAllObjects();


        createMenuButton();

        createMonitor();

        CC_BREAK_IF(!createMap());

        CC_BREAK_IF(!createPlayer());

        bRet = true;
    } while (0);

    return bRet;
    

}

bool CGameScene::createMap()
{
    // map
    m_pGameMap = CCTMXTiledMap::tiledMapWithTMXFile("material/map3.tmx");
    if (m_pGameMap)
    {
        this->addChild(m_pGameMap);
        return true;
    }
    else
    {
        return false;
    }
    
}

void CGameScene::createMonitor()
{
    // monitor
    m_pMonitor = new CMonitor(this);
    if (m_pMonitor)
    {
        addChild(m_pMonitor);
        m_pMonitor->autorelease();
        m_pMonitor->scheduleUpdate();
    }
}

bool CGameScene::canMoveForward( eMoveDirection direction, float moveDistance, CCSize objectSize, CCPoint &objectPos, bool bUpdate)
{
    bool bRet = false;

    CCSize mapSize = CGameScene::getMapSize();
    CCSize tileSize = CGameScene::getTileSize();


    switch (direction)
    {
    case MOVE_UP:
        if (objectPos.y <= mapSize.height - objectSize.height 
            && canPassThough(ccp(objectPos.x - tileSize.width/2, objectPos.y + objectSize.height / 2 + tileSize.height / 2), bUpdate)
            && canPassThough(ccp(objectPos.x + tileSize.width/2, objectPos.y + objectSize.height / 2 + tileSize.height / 2), bUpdate))
        {
            objectPos.y += moveDistance;
            bRet = true;
        }
        break;
    case MOVE_DOWN:
        if (objectPos.y >= objectSize.height 
            && canPassThough(ccp(objectPos.x - tileSize.width/2, objectPos.y - objectSize.height / 2 - tileSize.height / 2), bUpdate)
            && canPassThough(ccp(objectPos.x + tileSize.width/2, objectPos.y - objectSize.height / 2 - tileSize.height / 2), bUpdate))
        {
            objectPos.y -= moveDistance;
            bRet = true;
        }
        break;
    case MOVE_LEFT:
        if (objectPos.x >= objectSize.width 
            && canPassThough(ccp(objectPos.x - objectSize.width / 2 - tileSize.width / 2, 
            objectPos.y - tileSize.width/2), bUpdate)
            && canPassThough(ccp(objectPos.x - objectSize.width / 2 - tileSize.width / 2, 
            objectPos.y + tileSize.width/2), bUpdate))
        {
            objectPos.x -= moveDistance;
            bRet = true;
        }
        break;
    case MOVE_RIGHT:
        if (objectPos.x <= mapSize.width - objectSize.width 
            && canPassThough(ccp(objectPos.x + objectSize.width / 2 + tileSize.width / 2, 
            objectPos.y - tileSize.width/2), bUpdate)
            && canPassThough(ccp(objectPos.x + objectSize.width / 2 + tileSize.width / 2, 
            objectPos.y + tileSize.width/2), bUpdate))
        {
            objectPos.x += moveDistance;
            bRet = true;
        }
        break;
    default:
        break;
    }

    return bRet;
}

cocos2d::CCPoint CGameScene::correctPoint( const CCPoint &srcPt )
{
    CCPoint destPt;
    CCSize tileSize = CGameScene::getTileSize();

    destPt.x = int((srcPt.x + tileSize.width / 2) / tileSize.width) * tileSize.width;
    destPt.y = int((srcPt.y + tileSize.height / 2) / tileSize.height) * tileSize.height;

    return destPt;
}




