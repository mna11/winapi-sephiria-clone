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
    ITEM_INFO                   GetItemInfo() override      { return ITEM_INFO{ m_iID, ITEM_TYPE::STONE_TABLET, CStoneTabletData::GetInstance()->FindStoneTabletInfo(m_iID)->strName, CStoneTabletData::GetInstance()->FindStoneTabletInfo(m_iID)->strImg, CStoneTabletData::GetInstance()->FindStoneTabletInfo(m_iID)->iLeaf }; }
    const STONE_TABLET_INFO*    GetStoneTabletInfo() const  { return CStoneTabletData::GetInstance()->FindStoneTabletInfo(m_iID); }

private:
    vector<pair<int, int>>  m_vecRelativePos;

    // CItem을(를) 통해 상속됨
    void InitializeData(int iID, ITEM_TYPE eType) override;
};

