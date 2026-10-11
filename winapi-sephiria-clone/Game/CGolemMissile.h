#pragma once
#include "CMonster.h"
#include "CState.h"

enum class GOLEM_MISSILE_STATE
{
    DELAY,
    WARNING,
    EXPLOSION,
    END
};

enum class GOLEM_MISSLE_ATK_RECT
{
    MISSILE,
    END
};

class CGolemMissile :
    public CMonster, public CState<GOLEM_MISSILE_STATE>
{
public:
    CGolemMissile();
    ~CGolemMissile();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void UpdateTime() override;
    void Move() override;
    void Attack() override;
public:
    void ApplyChange() override;

public:
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override {}

public:
    void SetDelay(double dDelay) { m_dStartDelay = dDelay; }

private:
    double m_dStateTime;

    double m_dStartDelay;
    double m_dWarningTime;
    double m_dExplosionTime;
    double m_dEndTime;
};

