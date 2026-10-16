#pragma once

#include "CState.h"
#include "CMonster.h"

class CGolem;

enum class ERMA_STATE
{
    IDLE,
    WALK,
    AIR,
    STUN,
    STONE,
    RETURN,
    LIGHT,
    DOWN,
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

public:
    void UpdateFrame() override;
    void HitDamage(int iDamage, CObj* pObj = nullptr, HIT_SOURCE eHit = HIT_SOURCE::END, bool bCritical = false) override;
    void SetOwner(CGolem* pGolem) { m_pOwner = pGolem; }
    void SetFace(bool bRight);

public:
    void StartReturn();
    void StartExpose(VEC vPoint);
    bool IsReturnFinish();

public:
    void Show() { m_bView = true; }
    void Hide() { m_bView = false; }

private:
    void UpdateTime() override;
    void Move() override;
    void Attack() override;
    void Down() override;


private:
    VEC    m_vReturnPoint;
    VEC    m_vExposePoint;

    double m_dStateTime;
    CGolem* m_pOwner;
    bool    m_bView;

    bool    m_bFaceRight;
};
