#pragma once
#include "CScene.h"
class CEnd :
    public CScene
{
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
public:
    void Init_CreateObj() override;
private:
    double m_dEndTime = 8.f;
};

