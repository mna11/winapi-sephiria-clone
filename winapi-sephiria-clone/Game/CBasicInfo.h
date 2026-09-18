#pragma once
#include "CUI.h"
class CBasicInfo :
    public CUI
{
public:
    CBasicInfo();
    ~CBasicInfo();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

