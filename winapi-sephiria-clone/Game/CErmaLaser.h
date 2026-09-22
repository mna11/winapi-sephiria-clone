#pragma once

#include "CObj.h"

enum class ERMA_LASER_STATE
{
    DELAY,
    WARNING,
    ACTIVE
};

class CErmaLaser :
    public CObj
{
public:
    CErmaLaser();
    ~CErmaLaser();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void Configure(
        float fStartX,
        float fEndX,
        float fStartY,
        float fEndY,
        double dStartDelay,
        bool bFromLeft);

private:
    void ChangeState(ERMA_LASER_STATE eNextState);
    void UpdateLaserPosition();
    void UpdateHitBox();
    float GetBeamRatio() const;
    float GetBeamAlpha() const;
    void RenderWarning(Graphics* pGraphics, const VEC& vScroll) const;
    void RenderLaser(Graphics* pGraphics, const VEC& vScroll) const;

private:
    ERMA_LASER_STATE m_eState;
    float m_fLaserStartX;
    float m_fLaserEndX;
    float m_fLaserY;
    float m_fLaserStartY;
    float m_fLaserEndY;
    double m_dStateElapseTime;
    double m_dStartDelay;
    bool m_bFromLeft;
    bool m_bDamageApplied;
};
