#include "Controller.h"


CController::CController(CTank *tank)
    : m_pTank(tank)
{
}


CController::~CController(void)
{
}


bool CController::init(CCNode *parent)
{
    m_pTank->setController(this);

    //parent->addChild(this);

    autorelease();

    return true;
}

void CController::setTank( CTank *tank )
{
    m_pTank->removeChild(this, false);
    m_pTank = tank;
    m_pTank->addChild(this);
}

CTank* CController::getTank( void )
{
    return m_pTank;
}
