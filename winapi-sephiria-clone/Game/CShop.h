#pragma once

#include "CScene.h"

class CButton;
class CMsgBox;

class CShop :
    public CScene
{
public:
    CShop();
    ~CShop();
public:
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void Render(Graphics* pGraphics) override;
    void Release() override;
public:
    void Init_CreateObj() override;

public:
    void UpdateTime();

public:
    // 상점에서 없어지는거 일단 고려하지 않고 구현
    // 인벤토리 어디에 넣을려고 하는가, 어떤 아이템을 살려고 하는가
    
    // 인벤토리 UI에서 호출
    void TryBuyItem(int iInventoryIdx, int iID, ITEM_TYPE eItemType);
    void TrySellItem(int iInventoryIdx, int iID, ITEM_TYPE eItemType);

    // ShopMsgBox에서 호출
    void BuyItem(int iInventoryIdx, ITEM_INFO tItemInfo);
    void SellItem(int iInventoryIdx, ITEM_INFO tItemInfo);

private:
    int     m_iFrame;
    double  m_dFrameTime;

    CButton*    m_pEscapeButton;
    CMsgBox*    m_pMsgBoxUI;
    RectF       m_rcMsgBox;
};

