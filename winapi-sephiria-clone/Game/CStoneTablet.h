#pragma once
#include "CItem.h"
#include "CStoneTabletData.h"

class CStoneTablet :
    public CItem
{
public:
    CStoneTablet();
    ~CStoneTablet();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    ITEM_INFO                   GetItemInfo() override      { return CStoneTabletData::GetInstance()->FindItemInfo(m_iID); }
    const STONE_TABLET_INFO*    GetStoneTabletInfo() const  { return CStoneTabletData::GetInstance()->FindStoneTabletInfo(m_iID); }
public:
    void InitializeData(int iID, ITEM_TYPE eType) override;

private:
    vector<pair<int, int>>  m_vecRelativePos;
};

