#pragma once
#include "CObj.h"

class CItem :
    public CObj
{
public:
    CItem();
    ~CItem();

public:
    const int&                  GetLevel() const  { return m_iLevel; }
    const int&                  GetItemID() const { return m_iID; }
    const   ITEM_TYPE&          GetItemType() const { return m_eItemType; }
    virtual ITEM_INFO           GetItemInfo() PURE;

public:
    virtual void SetLevel() {}
    virtual void AddLevel() {}

public:
    virtual void InitializeData(int iID, ITEM_TYPE eType) PURE;

protected:
    int         m_iID;
    ITEM_TYPE   m_eItemType;
    int         m_iLevel;
};