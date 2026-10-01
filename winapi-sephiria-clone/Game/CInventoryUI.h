#pragma once
#include "CUI.h"
#include "CInventorySlotUI.h"

// 인벤토리 컨트롤러와 뷰어의 역할을 동시에 함

class CInventory;
class CItem;

class CInventoryUI :
    public CUI
{
public:
    CInventoryUI();
    ~CInventoryUI();
public:
    void    Initialize() override;
    int     Update() override;
    void    LateUpdate() override;
    void    Render(Graphics*) override;
    void    Release() override;

public:
    void    SetInventorySize(int iSize);
    void    SetInventory(CInventory* pInventory);

public:
    const vector<CInventorySlotUI*>&    GetItemSlots() const { return m_vecItemSlot; }

public:
    void    SyncInventorySlot();


private:
    vector<CInventorySlotUI*>   m_vecItemSlot;
    CInventory*                 m_pInventory;

    int                         m_iMouseHoverSlot;
    int                         m_iStartSlot;
};

