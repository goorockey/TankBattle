#include "stdio.h"
#include "GameScene.h"
#include "AIController.h"

#define CHOICE_CNT  5

CAIController::CAIController(CTank *tank)
    : CController(tank)
    , m_nMoveStep(0)
{
    srand((unsigned)time(NULL));
}


CAIController::~CAIController(void)
{
}

void CAIController::update( float dt )
{
    static CCSize mapSize = CGameScene::getMapSize();
    static CCSize tileSize = CGameScene::getTileSize();
    
    unsigned int i = rand() % CHOICE_CNT;

    if (m_pTank->isMoving() == false)
    {
        if (m_nMoveStep <= 0)
        {
            switch (i)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                m_pTank->move(eMoveDirection(MOVE_LEFT + i));
                m_nMoveStep = rand() % int(mapSize.height / tileSize.height);
                break;
            default:
                break;
            }
        }
        else 
        {
            m_nMoveStep--;
            eMoveDirection dir = m_pTank->getDirection();
            m_pTank->move(dir);
        }
    }


    i = rand() % 50;
    if (i == 0)
    {
        m_pTank->shoot();
    }
}
