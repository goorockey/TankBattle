#pragma once

#include "Controller.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#define KEY_DOWN(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 1 : 0)
#define KEY_UP(vk_code) (GetAsyncKeyState(vk_code) & 0x8000 ? 0 : 1)
#endif

class CKeyboardController : public CController
{
public:
    CKeyboardController(CTank *tank);
    ~CKeyboardController(void);

    virtual void update(float dt);

protected:
    bool    m_bShooting;

};


class CKeyboardController2 : public CController
{
public:
    CKeyboardController2(CTank *tank);
    ~CKeyboardController2(void);

    virtual void update(float dt);

protected:
    bool    m_bShooting;

};
