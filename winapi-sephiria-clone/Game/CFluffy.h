#pragma once
#include "CMonster.h"
#include "CState.h"

enum class FLUFFY_STATE
{
    SUMMON,
    IDLE,
    WALK,
    ATK,
    STUN, 
    DOWN,
    END
};

class CFluffy :
    public CMonster, public CState<FLUFFY_STATE>
{
public:
    CFluffy();
    ~CFluffy();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void ApplyChange() override;
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override;

private:
    void UpdateTime() override;
    void Move() override;
    void Attack() override;

private:
    double m_dStateTime;

    double m_dSummonTime;   // 소환 걸리는 시간
    double m_dAtkTime;      // 공격 걸리는 시간
    double m_dDownTime;     // 다운 상태로 남아있는 시간

    double m_dStunTime;
    double m_dAtkElapse;
    double m_dAtkCycle;
};

