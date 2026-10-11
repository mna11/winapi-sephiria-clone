#pragma once

#include "CState.h"
#include "CMonster.h"

enum class ERMA_STATE
{
    IDLE,
    WALK,
    AIR,
    STUN,
    STONE,
    RETURN,
    LIGHT,
    END
};

class CErma :
    public CMonster, public CState<ERMA_STATE>
{
public:
    CErma();
    ~CErma();

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
};
