#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class CGameManager
{
private:
    CGameManager(void);
    ~CGameManager(void);

public:
    enum SceneId
    {
        SCENE_START,
        SCENE_PLAY,
        SCENE_GAMEOVER
    };

    static CGameManager* getGameManager();
    static void purgeGameManger();

    bool init();

    void runSceneWithId(SceneId id);

    CCSpriteFrameCache* getFrameCache();

private:
    static CGameManager    *m_pGameManager;

    CCSpriteFrameCache   *m_pCache;

};

