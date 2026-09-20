#pragma once
#include "CScene.h"

class CBossStage :
    public CScene
{
public:
    CBossStage();
    ~CBossStage();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;

public:
    void Init_CreateObj() override;
    void Init_InsertImg() override;
};

