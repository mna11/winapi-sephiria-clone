#pragma once
#include "CMonster.h"
#include "CState.h"

class CGolem;

enum class GOLEM_HEAD_STATE
{
    IDLE,
    SHOOT,
    EXPOSE,
    RECOVER,
    DOWN,
    END
};

class CGolemHead :
    public CMonster, public CState<GOLEM_HEAD_STATE>
{
public:
    CGolemHead();
    ~CGolemHead();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
private:
    void UpdateFrame() override;
    void UpdateTime() override;
    void Move() override;
    void Attack() override;
private:
    void ApplyChange() override;
public:
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override;

public:
    void SetOwner(CGolem* pGolem) { m_pOwner = pGolem; }

private:
    double m_dStateTime; 
    CGolem* m_pOwner;
};

