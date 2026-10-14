#pragma once
#include "CItem.h"
#include "CPotionData.h"

class CPotion :
    public CItem
{
public:
    CPotion();
    ~CPotion();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    ITEM_INFO GetItemInfo() override { return CPotionData::GetInstance()->FindItemInfo(m_iID); }
    const POTION_INFO* GetPotionInfo() const { return CPotionData::GetInstance()->FindPotionInfo(m_iID); }
    const int& GetHave() const { return m_iHave; }
public:
    void SetHave(int iAmount);
    void AddHave(int iAmount);

public:
    void InitializeData(int iID, ITEM_TYPE eType) override;

private:
    int m_iHave; // 보유 중인 개수 -> 포션은 중복이 가능하므로
};

