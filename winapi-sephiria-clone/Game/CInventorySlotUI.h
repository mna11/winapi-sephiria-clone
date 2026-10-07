#pragma once
#include "CUI.h"

class CItem;

class CInventorySlotUI :
    public CUI
{
public:
    CInventorySlotUI();
    ~CInventorySlotUI();

public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetItem(CItem* pItem) { m_pItem = pItem; }
    void SetLevel(int iLevel)  { m_iSlotLevel = iLevel; }
    void SetCollide(bool bCol) { m_bCol = bCol; }

private:
    CItem*  m_pItem;
    int     m_iSlotLevel;
    bool    m_bCol;
};

