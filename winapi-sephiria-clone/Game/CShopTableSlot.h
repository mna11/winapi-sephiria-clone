#pragma once
#include "CUI.h"
class CShopTableSlot :
    public CUI
{
public:
    CShopTableSlot();
    ~CShopTableSlot();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void SetItemID(int iID) { m_iItemID = iID; }
    void SetItemType(ITEM_TYPE eItemType) { m_eItemType = eItemType; }
    void SetCollide(bool bCol) { m_bCol = bCol; }

public:
    const int& GetItemID() const         { return m_iItemID; }
    const ITEM_TYPE& GetItemType() const { return m_eItemType; }

private:
    int       m_iItemID;
    ITEM_TYPE m_eItemType;
    bool m_bCol;
};

