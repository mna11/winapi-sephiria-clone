#pragma once
#include "CObj.h"

class CMouse;

class CUI abstract:
    public CObj
{
public:
    CUI();
    virtual ~CUI();

public:
    void    Show();
    void    Hide();
    void    Toggle();

public:
    bool    GetView() const { return m_bView; }

public:
    void    SetMouse(CMouse* pMouse) { m_pMouse = pMouse; }

protected:
    bool    m_bView;
    CMouse* m_pMouse;
    float   m_fUIScale;
};

