#pragma once
#include "CUI.h"
#include "CState.h"

class CItemSelectSlotUI;

enum class ITEM_SELECT_UI_STATE
{
    READY,   // 스페이스바 누르기 대기
    BROKEN,  // 터지고 슬롯들 나오기
    MOVE,    // 왼쪽으로 이동하기
    SELECT,  // 플레이어가 고르기
    END
};

class CItemSelectUI :
    public CUI, public CState<ITEM_SELECT_UI_STATE>
{
public:
    CItemSelectUI();
    ~CItemSelectUI();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;

public:
    void Show() override;
    void Toggle() override;

public:
    void ApplyChange() override;

public:
    void UpdateTime();
    void KeyInput();
    void Move();
    void HandleCollisionMouse();

public:
    void GachaItems();

private:
    double m_dStateTime;
    double m_dBrokenTime;
    double m_dSlotRenderTime;

    const int   m_iItemNum;
    vector<CItemSelectSlotUI*> m_vecItemSlot;
};

