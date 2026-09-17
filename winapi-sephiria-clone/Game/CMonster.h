#pragma once
#include "CObj.h"
class CMonster :
    public CObj
{
public:
    CMonster();
    ~CMonster();
public:
    void SetDamage(int iDamage, CObj* pObj) override;
};

