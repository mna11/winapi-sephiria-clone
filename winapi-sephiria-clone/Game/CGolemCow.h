#pragma once
#include "CMonster.h"
#include "CState.h"

enum class GOLEM_COW_STATE
{
    SUMMON,
    IDLE,
    WALK, 
    READY,
    CHARGE,
    AIR,
    DOWN,
    END
};

enum class GOLEM_COW_ATK_RECT
{
    CHARGE,
    END
};

class CGolemCow :
    public CMonster, public CState<GOLEM_COW_STATE>
{
public:
    CGolemCow();
    ~CGolemCow();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit) override;
    void OnWallCollision() override;

private:
    void UpdateTime() override;
    void Move() override;
    void Attack() override;

private:
    void ApplyChange() override;

private:
    double m_dStateTime;
    
    double m_dSummonTime;
    double m_dDownTime;

    double m_dChargeReadyTime;
    double m_dChargeInterval;

    double m_dDustInterval;
    double m_dDustElapsedTime;
    VEC m_vDir;
};

