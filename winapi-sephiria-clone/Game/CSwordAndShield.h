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
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetTarget(CObj* pObj) override;

public:
    void Attack() override;
    void SpecialAttack() override;

public:
    void ApplyChange() override;

public:
    void CreateEffect();

private:
    void AttackUpdate();
    void DefenseUpdate();
    void CleaveUpdate();

private:
    CObj*   m_pSword;
    CObj*   m_pShield;

#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 3.;
#endif // DEBUG 
};

