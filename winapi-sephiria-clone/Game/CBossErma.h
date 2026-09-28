#pragma once

#include "CMonster.h"
#include "CState.h"

class CErma;
class CErmaBody;
class CErmaHand;
class CErmaHead;
class CErmaPhase;
class CErmaPhase1;

enum class BOSS_ERMA_STATE
{
    WAIT,
    INTRO,
    IDLE,
    ATTACK,
    EXPOSED,
    RECOVER,
    LASER,
    MISSILE,
    END
};

class CBossErma :
    public CMonster, public CState<BOSS_ERMA_STATE>
{
public:
    CBossErma();
    ~CBossErma();

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
    void InitializeParts();
    bool AddStaggerDamage(int iDamage);

public:
    CErmaHead* GetHead() const { return m_pHead; }
    CErmaBody* GetBody() const { return m_pBody; }
    CErmaHand* GetRightHand() const { return m_pRightHand; }
    CErmaHand* GetLeftHand() const { return m_pLeftHand; }
    CErma* GetErma() const { return m_pErma; }
    bool IsHit() const { return m_bHit; }

private:
    void UpdateTime();
    void ChangePhase(CErmaPhase* pNextPhase);
    bool IsTargetInArena() const;
    void StartMissileVolley();
    void StartLaserPattern();
    void BeginEnding();

private:
    CErmaHead* m_pHead;
    CErmaBody* m_pBody;
    CErmaHand* m_pRightHand;
    CErmaHand* m_pLeftHand;
    CErmaHand* m_pAttackingHand;
    CErma* m_pErma;

    CErmaPhase* m_pPhase;
    double m_dStateElapseTime;

    bool m_bNextLeftHand;
    int m_iNextPattern;
    bool m_bBattleStarted;
    bool m_bPartsInitialized;
    bool m_bEnding;

    friend class CErmaPhase1;
};
