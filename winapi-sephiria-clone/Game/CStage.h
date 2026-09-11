#pragma once
#include "CScene.h"
class CStage :
    public CScene
{
public:
    CStage();
    ~CStage();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
};

