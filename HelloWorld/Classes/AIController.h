#pragma once

#include "Controller.h"

class CAIController : public CController
{
public:
    CAIController(CTank *tank);
    ~CAIController(void);

    virtual void update(float dt);

protected:
    int m_nMoveStep;

};

