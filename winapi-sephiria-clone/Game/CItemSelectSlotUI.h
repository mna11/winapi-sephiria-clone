#pragma once
#include "CUI.h"
class CItemSelectSlotUI :
    public CUI
{
public:
    CItemSelectSlotUI();
    ~CItemSelectSlotUI();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void SetItemID(int iID) { m_iItemID = iID; }
    void SetSpread(bool bSpread) { m_bSpread = bSpread; }
    void SetDirVector(VEC vDir) { m_vDir = vDir; }
    void SetDistance(float fDistance) { m_fDistance = fDistance; }
    void SetCollide(bool bCol) { m_bCol = bCol; }

public:
    const int& GetItemID() const { return m_iItemID; }
    const VEC& GetDirVector() const { return m_vDir; }
    const float& GetDistance() const { return m_fDistance; }

public:
    void Move();

private:
    int     m_iItemID;      // 현재 슬롯에 나타낼 아이템
    bool    m_bSpread;      // 퍼지기 가능?
    float   m_fDistance;    // SelectSlotUI의 vPoint와의 최대 거리
    VEC     m_vDir;         // SelectSlotUI의 vPoint에서의 각도

    bool    m_bCol;         // 마우스 충돌 여부
};

