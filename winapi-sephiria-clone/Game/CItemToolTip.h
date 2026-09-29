#pragma once
#include "CUI.h"
class CItemToolTip :
    public CUI
{
public:
    CItemToolTip();
    ~CItemToolTip();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

