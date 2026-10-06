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
public:
    void SetWeaponID(int iID) { m_iWeaponID = iID; }
    void SetCollide(bool bCol) { m_bCol = bCol; }
public:
    const int& GetWeaponID() const { return m_iWeaponID; }

private:
    int     m_iWeaponID;    // 슬롯에 표시할 무기 아이디
    bool    m_bCol;         // 마우스 충돌 여부
};

