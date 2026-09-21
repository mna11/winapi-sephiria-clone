#pragma once
#include "CUI.h"
class CBossHp :
    public CUI
{
public:
    CBossHp();
    ~CBossHp();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

