#pragma once
#include "CUI.h"

class CForgeSlotUI;

class CForgeUI :
    public CUI
{
public:
    CForgeUI();
    ~CForgeUI();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void Show() override;

private:
    void UpdateWeaponList();
    void HandleCollisionMouse();

private:
    vector<CForgeSlotUI*>   m_vecWeaponSlot;
};

