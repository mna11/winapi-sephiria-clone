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
    void            SetDragItem(CItem* pItem) { m_pDragItem = pItem; }
    const CItem*    GetDragItem() const { return m_pDragItem; }

private:
    void UpdateTime();
    void UpdatePoint();
    void UpdateClick();

private:
    double m_dStateTime;
    double m_dClickTime; // 클릭 업 애니메이션 타임

    CItem* m_pDragItem;
    ImageAttributes m_imgAttrTranslucent;
};

