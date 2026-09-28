#pragma once
#include "CObj.h"
class CMonster abstract :
    public CObj
{
public:
    CMonster();
    virtual ~CMonster();

protected:
    ImageAttributes m_imgAttrHit;
    ImageAttributes m_imgAttrDown;
};

