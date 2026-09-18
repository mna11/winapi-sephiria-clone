#pragma once
#include "CMonster.h"
#include "CState.h"

enum class FLUFFY_STATE
{
    SUMMON,
    IDLE,
    WALK,
    ATTACK,
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
    void UpdateFrame() override;

public:
    void ApplyChange() override;
    void SetDamage(int iDamage, CObj* pObj) override;

private:
    void UpdateTime();
    void Move();
    void Attack();

private:
    double m_dAtkInterval;
    double m_dAtkElapseTime;
};

