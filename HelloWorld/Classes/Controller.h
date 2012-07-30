#pragma once

#include "Tank.h"

class CController : public CCNode
{
public:
    CController(CTank *tank);
    ~CController(void);

    virtual bool init(CCNode *parent);

    virtual void update(float dt) = 0;

    void setTank(CTank *tank);
    CTank* getTank(void);

protected:
    CTank   *m_pTank;
};

