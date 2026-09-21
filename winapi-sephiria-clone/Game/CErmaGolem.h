#pragma once
#include "CMonster.h"
#include "CState.h"

class CErma;
class CErmaHand;

enum class ERMA_GOLEM_STATE
{
    WAIT,
    INTRO,
    IDLE,
    LEFT_HAND_ATTACK,
    RIGHT_HAND_ATTACK,
    EXPOSED,
    RECOVER,
    END
};

class CErmaGolem :
    public CMonster, public CState<ERMA_GOLEM_STATE>
{
public:
    CErmaGolem();
    ~CErmaGolem();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void ApplyChange() override;
    void SetDamage(int iDamage, CObj* pObj = nullptr) override;

public:
    void SetParts(CErma* pErma, CErmaHand* pLeftHand, CErmaHand* pRightHand);
    void AddStaggerDamage(int iDamage);

private:
    void CheckBattleStart();
    void UpdateTime();
    void BeginEnding();

private:
    CErma* m_pErma;
    CErmaHand* m_pLeftHand;
    CErmaHand* m_pRightHand;

    double m_dStateElapseTime;
    bool m_bNextLeftHand;
    bool m_bBattleStarted;
    bool m_bEnding;
};
