#pragma once
#include "CObj.h"
#include "CState.h"

class CItem;

enum class MOUSE_STATE
{
    COMBAT,
    UI_IDLE,
    UI_CLICK_DOWN,
    UI_CLICK_UP,
    END
};

class CMouse :
    public CObj, public CState<MOUSE_STATE>
{
public:
    CMouse();
    ~CMouse();
public:
    void Initialize() override;
    int Update() override;
    void LateUpdate() override;
    void Render(Graphics*) override;
    void Release() override;
public:
    void ApplyChange() override;

public:
    void                            SetDragReferItem(MOUSE_REFER_ITEM eReferItem)   { m_tDragReferItem = eReferItem; }
    void                            SetHoverReferItem(MOUSE_REFER_ITEM eReferItem)  { m_tHoverReferItem = eReferItem; }
    void                            SetHoverItem(CItem* pItem) { m_pHoverItem = pItem; }

public:
    const MOUSE_REFER_ITEM&         GetDragReferItem()   const   { return m_tDragReferItem; }
    const MOUSE_REFER_ITEM&         GetHoverReferItem()  const   { return m_tHoverReferItem; }
    CItem*                          GetHoverItem() { return m_pHoverItem; }

private:
    void UpdateTime();
    void UpdatePoint();
    void UpdateClick();

private:
    double m_dStateTime;
    double m_dClickTime; // 클릭 업 애니메이션 타임

    CItem*           m_pHoverItem;
    MOUSE_REFER_ITEM m_tHoverReferItem;
    MOUSE_REFER_ITEM m_tDragReferItem;

    ImageAttributes m_imgAttrTranslucent;
};

