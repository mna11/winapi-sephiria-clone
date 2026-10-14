#pragma once
#include "CUI.h"
class CPotionToolTip :
    public CUI
{
public:
    CPotionToolTip();
    ~CPotionToolTip();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

