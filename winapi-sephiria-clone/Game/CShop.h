#pragma once

#include "CScene.h"

class CShop :
    public CScene
{
public:
    CShop();
    ~CShop();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
public:
    void Init_CreateObj() override;

private:
    int    m_iFrame = 0;
    double m_dFrameTime = 0.0;
};

