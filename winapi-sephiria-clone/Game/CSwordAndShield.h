#pragma once

#include "CWeapon.h"
#include "CState.h"

enum class SWORD_AND_SHIELD_STATE
{
    IDLE,
    ATTACK,
    SHIELD,
    CLEAVE,
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

private:
    CObj*   m_pSword;
    CObj*   m_pShield;

  
    // 임시 - 테스트 중
    RECT    m_tSwordRect;
    PointF  m_tBarriorRect[4];

#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 1.;
#endif // DEBUG 
};

