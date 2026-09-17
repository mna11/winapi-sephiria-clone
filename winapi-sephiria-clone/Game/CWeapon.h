#pragma once
#include "CObj.h"

class CWeapon abstract :
    public CObj
{
public:
    enum class TYPE {
        NONE,
        SwordAndShield,
        Greatsword,
        Dagger,
        Crossbow,
        Katana,
        Staff
    };

public:
    CWeapon();
    virtual ~CWeapon();
public:
    virtual void    Attack()        PURE;  // 일반 좌클릭
    virtual void    SpecialAttack() PURE;  // 일반 우클릭
public:
    void            SetAtk(int iAtkLvl, int iAtkMax, double dAtkElapseTime, double dAtkDuringTime, double dComboTime)
    {
        // NextAtk 플래그는 시작은 false로 고정이니 함수 파라미터에서 제외
        m_tAtk = { iAtkLvl, iAtkMax, false, dAtkElapseTime, dAtkDuringTime, dComboTime };
    }

protected:
    ATK_INFO m_tAtk;
};

