#pragma once
#include "CScene.h"
class CLibLoading :
    public CScene
{
public:
    CLibLoading();
    ~CLibLoading();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
    void Init_CreateObj() override;
private:
    int    m_iFrame = 0;
    double m_dFrameTime = 0.0;

    double m_dNextStageTime = 5.f;
};

