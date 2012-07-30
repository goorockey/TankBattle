#include "Base.h"
#include "GameScene.h"
#include "Bullet.h"

const char szBaseImg[] = "base.png";

CBase::CBase(void)
    : m_pBase(NULL)
    , m_bActive(false)
{
}


CBase::~CBase(void)
{
}

bool CBase::init( CCNode *parent )
{
    bool bRet = false;

    do 
    {
        CCSprite *base = CCSprite::spriteWithFile(szBaseImg);
        CC_BREAK_IF(!base);

        setActive(false);

        m_pBase = base;

        this->addChild(base);
        parent->addChild(this);

        bRet = true;
    } while (0);

    return bRet;
}

bool CBase::isCollide( CBullet *bullet )
{
    if (getActive() == false)
    {
        return false;
    }

    CCRect baseRect = CGameScene::calcSpriteRect(m_pBase);
    CCRect bulletRect = CGameScene::calcSpriteRect(bullet->getBullet());

    return CCRect::CCRectIntersectsRect(baseRect, bulletRect);
}

void CBase::setActive( bool active )
{
    m_bActive = active;

    if (m_pBase)
    {
        m_pBase->setVisible(active);
    }
}

bool CBase::getActive( void )
{
    return m_bActive;
}

CBase* CBase::createBase( CCNode *parent )
{
    CBase *base = new CBase;
    if (base && base->init(parent))
    {
        base->autorelease();
        return base;
    }
    else
    {
        CC_SAFE_RELEASE(base);
        return NULL;
    }
}

void CBase::setPosition( CCPoint pt )
{
    m_pBase->setPosition(pt);
}

const CCPoint& CBase::getPosition( void )
{
    return m_pBase->getPosition();
}
