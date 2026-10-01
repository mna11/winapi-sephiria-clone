#pragma once

#include "CScene.h"

class CUI;
class CButton;

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
    // 상점에서 없어지는거 일단 고려하지 않고 구현
    // 인벤토리 어디에 넣을려고 하는가, 어떤 아이템을 살려고 하는가
    
    // 인벤토리 UI에서 호출
    void TryBuyItem(int iInventoryIdx, int iID);
    void TrySellItem(int iInventoryIdx);

    // ShopMsgBox에서 호출
    void BuyItem();
    void SellItem();

private:
    int     m_iFrame = 0;
    double  m_dFrameTime = 0.0;

    CButton*    m_pEscapeButton;
    CUI*        m_pMsgBoxUI;
};

