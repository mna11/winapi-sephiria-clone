#pragma once
#include "CObj.h"
class CItem :
    public CObj
{
public:
    CItem();
    ~CItem();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void InitializeData(int iID);
    void UpdateData();

public:
    const int& GetLevel() const { return m_iLevel; }

public:
    void SetItemInfo(int iID, int iMaxLevel, wstring strImg, wstring strName, wstring strDescription, ITEM_CATEGORY eCategory)
    {
        m_tItemInfo = { iID, iMaxLevel, strImg, strName, strDescription, eCategory };
    }
  
public:
    void AddLevel(int iLevel);

public:
    const ITEM_INFO& GetItemInfo() const { return m_tItemInfo; }

private:
    int       m_iLevel;
    ITEM_INFO m_tItemInfo;
};

