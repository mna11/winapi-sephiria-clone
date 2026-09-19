#pragma once
#include "CObj.h"
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

protected:
    bool    m_bView;
};

