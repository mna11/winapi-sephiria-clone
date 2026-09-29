#pragma once
#include "CUI.h"
#include "CInventorySlotUI.h"

class CInventory;
class CItem;

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

public:
    void                    SetInventorySize(int iSize);
    void                    SetInventory(CInventory* pInventory) { m_pInventory = pInventory; }
public:
    const vector<CInventorySlotUI*>&    GetItemSlots() const { return m_vecItemSlot; }

private:
    vector<CInventorySlotUI*> m_vecItemSlot;
    CInventory* m_pInventory;

    int     m_iMouseHoverSlot;
    int     m_iStartSlot;
    bool    m_bDrag;
};

