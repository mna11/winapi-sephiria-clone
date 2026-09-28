#pragma once
#include "CUI.h"
class CInventoryUI :
    public CUI
{
public:
    CInventoryUI();
    ~CInventoryUI();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

