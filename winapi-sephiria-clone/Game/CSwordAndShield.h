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
    const RECT& GetAtkRect() const { return m_tAtkRect; }
    const RECT& GetDefRect()    const { return m_tDefRect; }

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

    RECT    m_tAtkRect;     // 공격 시 발생하는 충돌 렉트
    RECT    m_tDefRect;     // 방어 시 발생하는 충돌 렉트
    RECT    m_tClvRect;     // 회전 시 발생하는 충돌 렉트
#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 1.;
#endif // DEBUG 
};

