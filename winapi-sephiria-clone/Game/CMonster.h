#pragma once
#include "CObj.h"
class CMonster abstract :
    public CObj
{
public:
    CMonster();
    virtual ~CMonster();
public:
    void SetDamage(int iDamage, CObj* pObj) override;
};

