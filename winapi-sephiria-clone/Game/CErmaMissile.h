#pragma once

#include "CObj.h"

enum class ERMA_MISSILE_STATE
{
    DELAY,
    WARNING,
    STRIKE
};

class CErmaMissile :
    public CObj
{
public:
    CErmaMissile();
    ~CErmaMissile();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void Configure(
        const VEC& vImpactPoint,
        double dStartDelay);

private:
    void ChangeState(ERMA_MISSILE_STATE eNextState);
    void TryDamageTarget();
    void RenderWarning(Graphics* pGraphics, const VEC& vScroll) const;
    void RenderStrike(Graphics* pGraphics, const VEC& vScroll) const;

private:
    ERMA_MISSILE_STATE m_eState;
    VEC m_vImpactPoint;
    double m_dStateElapseTime;
    double m_dStartDelay;
    bool m_bDamageApplied;
};
