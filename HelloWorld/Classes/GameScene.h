#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "common.h"
using namespace cocos2d;

class CMonitor;


class CGameScene : public cocos2d::CCLayer
{
    CGameScene();
    ~CGameScene();

public:

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    void createMonitor();

    bool createMap();

    bool createPlayer();

    void createMenuButton();

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    void menuNewGameCallback(CCObject* pSender);


    // implement the "static node()" method manually
    CREATE_FUNC(CGameScene);

    CCArray* getPlayerList();

    bool startNewGame();


    static bool setWall(CCPoint &pt);

public:
    static CCPoint tileCoordForPosition(CCPoint &position);    // 把坐标转换为左上角为(0,0）的格子坐标
    static CCSize getMapSize();
    static CCSize getTileSize();
    static float calcDistance(const CCPoint &pt1, const CCPoint &pt2);
    static bool canPassThough(CCPoint point, bool bUpdate = false);   // 判断是否能穿越
    static bool canMoveForward( eMoveDirection direction, float moveDistance, CCSize objectSize, CCPoint &objectPos, bool bUpdate = false);
    static CCPoint correctPoint(const CCPoint &pt);
    static CCRect calcSpriteRect(CCSprite *sprite);
    static void gameOver();

private:
    static CCTMXTiledMap    *m_pGameMap;
    CMonitor    *m_pMonitor;
    CCArray     *m_PlayerList;
};

#endif // __HELLOWORLD_SCENE_H__
