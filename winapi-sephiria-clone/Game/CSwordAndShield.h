#pragma once

#include "CWeapon.h"
#include "CState.h"

enum class SWORD_AND_SHIELD_STATE
{
    IDLE,
    ATTACK,
    DEFENSE,
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

private:
    CObj*   m_pSword;
    CObj*   m_pShield;

    RECT    m_tAtkRect;     // 공격 시 발생하는 충돌 렉트
    RECT    m_tDefRect;     // 방어 시 발생하는 충돌 렉트
    CObj*   m_pDefenceEffect; // 누를 때는 살려뒀다가, 땔 때, 죽여야하므로 기억해둠
                              // 공격은 기억 안하고, 방어만 기억하는 이유는 방어도 공격처럼하면 이펙트가 매 프레임 생성 소멸 해야하기 때문 
#ifdef _DEBUG
private:
    void    PrintInfo();
    double  m_dPrintInterval = 1.;
#endif // DEBUG 
};

