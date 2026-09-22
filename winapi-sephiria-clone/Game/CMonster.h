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

    // 공격 기준 - 플레이어와의 거리
    float m_fAtkDistance;
};

