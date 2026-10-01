#pragma once
#include "CUI.h"

class CShopTableSlot;

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

public:
    void SetSellingItem();

private:
    vector<CShopTableSlot*>     m_vecItemSlot;

    const int                   m_iItemSlotSize;

    int                         m_iMouseHoverSlot;
};

