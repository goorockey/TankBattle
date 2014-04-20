#include "GameManager.h"
#include "GameOver.h"


using namespace cocos2d;

CCScene* CGameOver::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CGameOver *layer = (CGameOver*)CGameOver::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CGameOver::init()
{
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        CCSize size = CCDirector::sharedDirector()->getWinSize();

        // background
        CCSprite *bg = CCSprite::spriteWithFile("background.png");
        CC_BREAK_IF(!bg);

        bg->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(bg);


        // image
        CCSprite *girl = CCSprite::spriteWithFile("girl4.png");
        girl->setPosition(ccp(size.width/4, size.height/2));
        this->addChild(girl);


        // label 'Game Over'
        CCLabelTTF* pLabel = CCLabelTTF::labelWithString("GAME OVER", "Arial", 30);
        CC_BREAK_IF(! pLabel);

        pLabel->setColor(ccRED);
        pLabel->setPosition(ccp(size.width/2, size.height/2 + 100));

        this->addChild(pLabel);


        // 'Retry'
        pLabel = CCLabelTTF::labelWithString("Retry", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        CCMenuItemLabel *pRetryItem = CCMenuItemLabel::itemWithLabel(
            pLabel,
            this,
            menu_selector(CGameOver::menuRetryCallback));
        CC_BREAK_IF(! pRetryItem);

        pRetryItem->setPosition(ccp(size.width/2, size.height/2+20));


        // 'Exit'
        pLabel = CCLabelTTF::labelWithString("Exit", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        CCMenuItemLabel *pExitItem = CCMenuItemLabel::itemWithLabel(
            pLabel,
            this,
            menu_selector(CGameOver::menuExitCallback));

        CC_BREAK_IF(! pExitItem);

        pExitItem->setPosition(ccp(size.width/2, size.height/2-20));

        CCMenu* pMenu = CCMenu::menuWithItems(pRetryItem, pExitItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        this->addChild(pMenu, 1);

        bRet = true;
    } while (0);

    return bRet;
}

void CGameOver::menuRetryCallback(CCObject* pSender)
{
    // "Retry" menu item clicked
    CGameManager::getGameManager()->runSceneWithId(CGameManager::SCENE_PLAY);
}

void CGameOver::menuExitCallback(CCObject* pSender)
{
    // "Exit" menu item clicked
    //CGameManager::runSceneWithId(GameManager::SCENE_MENU);
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

