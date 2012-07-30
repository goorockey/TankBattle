#include "Bullet.h"
#include "GameScene.h"


const char szBulletImg[] = "material/bullet.png";

CBullet::CBullet(eMoveDirection direction)
    : m_Direction(direction)
{
}


CBullet::~CBullet()
{
}

void CBullet::setDirection( eMoveDirection direction )
{
    m_Direction = direction;
}

eMoveDirection CBullet::getDireciton()
{
    return m_Direction;
}

CBullet* CBullet::createBullet( CCNode *parent, eMoveDirection direction )
{
    CBullet *bullet = new CBullet(direction);

    if (bullet && bullet->init(parent))
    {
        bullet->autorelease();
        return bullet;
    }
    else
    {
        return NULL;
    }
}

bool CBullet::init( CCNode *parent)
{
    bool bRet = false;


    do 
    {
        CCSize tileSize = CGameScene::getTileSize();

        //CCSprite *bullet = CCSprite::spriteWithFile(szBulletImg, CCRectMake(0,0,tileSize.width*2, tileSize.height*2));
        CCSprite *bullet = CCSprite::spriteWithFile(szBulletImg);
        CC_BREAK_IF(!bullet);

        this->addChild(bullet);
        parent->addChild(this);


        m_pBullet = bullet;

        bRet = true;
    } while (0);

    return bRet;
}

const CCPoint& CBullet::getPosition()
{
    return m_pBullet->getPosition();
}

void CBullet::setPosition( const CCPoint& pos )
{
    m_pBullet->setPosition(pos);
}

CCSprite* CBullet::getBullet()
{
    return m_pBullet;
}

cocos2d::CCSize CBullet::getBulletSize()
{
    return m_pBullet->getContentSize();
}
