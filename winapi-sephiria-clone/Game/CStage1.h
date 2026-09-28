#pragma once
#include "CScene.h"
class CStage1 :
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
};

