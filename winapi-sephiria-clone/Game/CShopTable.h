#pragma once
#include "CUI.h"
class CShopTable :
    public CUI
{
public:
    CShopTable();
    ~CShopTable();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
};

