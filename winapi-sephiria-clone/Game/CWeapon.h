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
    virtual void Attack()        PURE;  // 일반 좌클릭
    virtual void SpecialAttack() PURE;  // 일반 우클릭
};

