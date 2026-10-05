#pragma once
#include "CUI.h"
class CForgeSlotUI :
    public CUI
{
public:
    CForgeSlotUI();
    ~CForgeSlotUI();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

