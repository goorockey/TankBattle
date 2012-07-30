#include "GameStart.h"
#include "GameManager.h"


CCScene* CGameStart::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        CGameStart *layer = CGameStart::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

bool CGameStart::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(!CCLayer::init());

        CCSize size = CCDirector::sharedDirector()->getWinSize();

        // background
        CCSprite *bg = CCSprite::spriteWithFile("background.png");
        CC_BREAK_IF(!bg);

        bg->setPosition(ccp(size.width/2, size.height/2));
        this->addChild(bg);

        // 'Tank Battle'
        CCLabelTTF* pLabel = CCLabelTTF::labelWithString("Tank Battle", "Arial", 60);
        CC_BREAK_IF(!pLabel);
        pLabel->setColor(ccRED);
        pLabel->setPosition(ccp(size.width/2, size.height/2 + 100));
        this->addChild(pLabel);

        // 'play game'
        pLabel = CCLabelTTF::labelWithString("Play Game", "Arial", 24);
        CC_BREAK_IF(! pLabel);

        CCMenuItemLabel *pPlayItem = CCMenuItemLabel::itemWithLabel(
            pLabel, this,
            menu_selector(CGameStart::menuPlayCallback));

        CC_BREAK_IF(! pPlayItem);

        pPlayItem->setPosition(ccp(size.width/2, size.height/2));

        CCMenu* pMenu = CCMenu::menuWithItems(pPlayItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        this->addChild(pMenu, 1);

        bRet = true;
    } while (0);

    return bRet;
}


void CGameStart::menuPlayCallback(CCObject* pSender)
{
    // "Play" menu item clicked
    CGameManager::getGameManager()->runSceneWithId(CGameManager::SCENE_PLAY);

}
