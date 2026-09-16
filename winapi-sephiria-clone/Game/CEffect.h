#pragma once
#include "CObj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    ~CEffect();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void UpdateFrame() override;

public:
    void SetAlpha(float fAlpha) { m_fAlpha = fAlpha; }

private:
    float m_fAlpha;
};

