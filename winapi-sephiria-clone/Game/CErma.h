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
    void UpdateFrame() override;

public:
    void ApplyChange() override;
    void SetDamage(int iDamage, CObj* pObj) override;

public:
    void SetAnchor(const VEC& vAnchor);
    void SetManagedByGolem(bool bManaged) { m_bManagedByGolem = bManaged; }
    void SetExposed(bool bExposed);
    void SetVisible(bool bVisible) { m_bVisible = bVisible; }
    void StartExposure(const VEC& vHeadPoint, const VEC& vStunPoint);
    void RequestRemove() { m_bRemoveRequested = true; }

public:
    bool IsDefeated() const { return m_bDefeated; }
    bool IsReturnComplete() const { return m_bReturnComplete; }

private:
    void UpdateTime();
    void Move();

private:
    VEC m_vAnchor;
    VEC m_vAirStart;
    VEC m_vAirTarget;
    VEC m_vReturnStart;

    double m_dStateElapseTime;
    int m_iExposureHitCount;

    bool m_bHasAnchor;
    bool m_bManagedByGolem;
    bool m_bExposed;
    bool m_bVisible;
    bool m_bReturnComplete;
    bool m_bFinalStone;
    bool m_bDefeated;
    bool m_bRemoveRequested;
};
