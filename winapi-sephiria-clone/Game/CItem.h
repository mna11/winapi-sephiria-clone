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
    void UpdateData(int iID);
public:
    void SetItemInfo(int iID, wstring strImg, wstring strName, wstring strDescription, ITEM_CATEGORY eCategory)
    {
        m_tItemInfo = { iID, strImg, strName, strDescription, eCategory };
    }
public:
    const ITEM_INFO& GetItemInfo() const { return m_tItemInfo; }

private:
    ITEM_INFO m_tItemInfo;
};

