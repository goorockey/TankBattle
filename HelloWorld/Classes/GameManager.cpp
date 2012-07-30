#include "GameManager.h"
#include "GameScene.h"
#include "GameStart.h"
#include "GameOver.h"

CGameManager* CGameManager::m_pGameManager = NULL;


CGameManager::CGameManager(void)
    : m_pCache(NULL)
{
}


CGameManager::~CGameManager(void)
{
}

void CGameManager::runSceneWithId( SceneId id )
{
    CCScene *newScene = NULL;

    switch (id)
    {
    case SCENE_START:
        newScene = CGameStart::scene();
        break;

    case SCENE_PLAY:
        newScene = CGameScene::scene();
        break;

    case SCENE_GAMEOVER:
        newScene = CGameOver::scene();
        break;

    }

    if (newScene)
    {
        if (CCDirector::sharedDirector()->getRunningScene() == NULL)
            CCDirector::sharedDirector()->runWithScene(newScene);
        else
            CCDirector::sharedDirector()->replaceScene(newScene);

    }
}

CGameManager* CGameManager::getGameManager()
{
    if (m_pGameManager == NULL)
    {
        m_pGameManager = new CGameManager;
        if (!m_pGameManager || !m_pGameManager->init())
        {
            CC_SAFE_DELETE(m_pGameManager);
        }
    }

    return m_pGameManager;
}


CCSpriteFrameCache* CGameManager::getFrameCache()
{
    return m_pCache;
}

bool CGameManager::init()
{
    bool bRet = false;

    do 
    {
        // frame cache
        m_pCache = CCSpriteFrameCache::sharedSpriteFrameCache();

        bRet = true;
    } while (0);

    return bRet;
}

void CGameManager::purgeGameManger()
{
    CC_SAFE_DELETE(m_pGameManager);
}
