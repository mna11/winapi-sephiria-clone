#pragma once
#include "CObj.h"
#include "CItemData.h"

class CItem :
    public CObj
{
public:
    CItem();
    ~CItem();
public:
    void Initialize() override;
    int  Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void InitializeData(int m_iID);

public:
    const int& GetLevel() const { return m_iLevel; }

public:
    void ChangeLevel(int iLevel);

public:
    const ITEM_INFO* GetItemInfo() const { return CItemData::GetInstance()->FindItemInfo(m_iID); }

private:
    int m_iID;
    int m_iLevel;
};

