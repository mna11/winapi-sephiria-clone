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

public:
    bool    GetView() const { return m_bView; }

protected:
    bool    m_bView;
};

