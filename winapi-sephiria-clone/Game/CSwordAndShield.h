#pragma once

#include "CWeapon.h"
#include "CState.h"

enum class SWORD_AND_SHIELD_STATE
{
    IDLE,
    ATTACK,
    DEFENSE,
    CLEAVE_READY,
    CLEAVE,
    END
};

enum class SWORD_AND_SHIELD_ATK_RECT
{
    ATTACK,
    CLEAVE,
    END
};

enum class SWORD_AND_SHIELD_DEF_RECT
{
    DEFENSE,
    END
};


class CSwordAndShield :
    public CWeapon, public CState<SWORD_AND_SHIELD_STATE>
{
public:
    CSwordAndShield();
    ~CSwordAndShield();

public:
    int Update() override;
    void LateUpdate() override;
    void Release() override;

public:
    void SetTarget(CObj* pObj) override;

public:
    void Attack() override;
    void SpecialAttack() override;

public:
    virtual void CreateEffect() PURE;

protected:
    virtual void AttackUpdate();
    virtual void DefenseUpdate();
    virtual void CleaveUpdate();

protected:
    CObj*   m_pSword;
    CObj*   m_pShield;

#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 3.;
#endif // DEBUG 
};

