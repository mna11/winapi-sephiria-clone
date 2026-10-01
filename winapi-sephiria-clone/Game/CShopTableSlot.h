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
    void SetCollide(bool bCol) { m_bCol = bCol; }

public:
    const int& GetItemID() const { return m_iItemID; }

private:
    int  m_iItemID;
    bool m_bCol;
};

