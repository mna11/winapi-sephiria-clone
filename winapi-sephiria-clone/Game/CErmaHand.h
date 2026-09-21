#pragma once

#include "CMonster.h"
#include "CState.h"

class CBossErma;

enum class ERMA_HAND_SIDE
{
    LEFT,
    RIGHT
};

enum class ERMA_HAND_STATE
{
    WAIT,
    READY,
    SLAM,
    RETURN,
    BROKEN,
    END
};

class CErmaHand :
    public CMonster, public CState<ERMA_HAND_STATE>
{
public:
    CErmaHand();
    ~CErmaHand();

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
    void SetSide(ERMA_HAND_SIDE eSide) { m_eSide = eSide; }
    void SetOwner(CBossErma* pOwner) { m_pOwner = pOwner; }
    void SetAnchor(const VEC& vAnchor);
    void StartSlam(const VEC& vTargetPoint);
    void ForceBreak();
    void Restore();
    void RequestRemove() { m_bRemoveRequested = true; }

public:
    bool IsBusy() const;

private:
    void UpdateTime();
    void Move();
    void RenderAttackEffect(Graphics* pGraphics, const VEC& vScroll);

private:
    ERMA_HAND_SIDE m_eSide;
    CBossErma* m_pOwner;

    VEC m_vAnchor;
    VEC m_vMoveStart;
    VEC m_vSlamTarget;

    double m_dStateElapseTime;
    bool m_bRemoveRequested;
};
