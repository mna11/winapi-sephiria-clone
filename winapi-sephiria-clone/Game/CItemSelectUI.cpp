#include "pch.h"
#include "CItemSelectUI.h"

#include "CMouse.h"

#include "CButton.h"

#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CItemSelectSlotUI.h"
#include "CTimeMgr.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CKeyMgr.h"
#include "CFontMgr.h"
#include "CSoundMgr.h"

CItemSelectUI::CItemSelectUI()
    : m_iItemNum(5), CState(ITEM_SELECT_UI_STATE::END, ITEM_SELECT_UI_STATE::READY),
    m_dBrokenTime(3.0), m_dStateTime(0.), m_dSlotRenderTime(2.0),
    m_pRerollBtn(nullptr), m_pSkipBtn(nullptr)
{
    m_vecItemSlot.resize(5, nullptr);
}

CItemSelectUI::~CItemSelectUI()
{
    Release();
}

void CItemSelectUI::Initialize()
{
    m_tInfo = { WINCX >> 1, WINCY >> 1, 0.f, 0.f };
    m_fSpeed = 0.f; 
    m_fUIScale = PIXEL_SCALE;

    m_pFrameKey = L"Sephirite_Broken";
    m_eCurState = ITEM_SELECT_UI_STATE::END;
    m_eNextState = ITEM_SELECT_UI_STATE::READY;

    // 렌더 정보 초기화 
    m_eRender = RENDERID::UI;
    m_iRenderLayer = 0;
}

int CItemSelectUI::Update()
{
    if (m_bDead)
        return DEAD;
    if (!m_bView)
        return NOEVENT;

    m_vPrePoint = m_tInfo.vPoint;

    // ApplyChange보다 UpdateTime이 먼저 왔더니, Broken에서 0번째 스프라이트가 한번 보이는거임
    // 왜지하고 생각해보니깐 ApplyChange에서 상태 시간을 0으로 초기화하기때문에
    // UpdateTime이 이론상 같은 프레임이지만 이전 프레임을 처리하고 있던 느낌이었던거임

    ApplyChange();

    UpdateTime();
    KeyInput();
    Move();

    for (auto& itemSlot : m_vecItemSlot)
        itemSlot->Update();

    HandleCollisionMouse();

    __super::UpdateFrame();
    return NOEVENT;
}

void CItemSelectUI::LateUpdate()
{
    if (!m_bView)
        return;

    for (auto& itemSlot : m_vecItemSlot)
        itemSlot->LateUpdate();
}

void CItemSelectUI::Render(Graphics* pGraphics)
{
    if (!m_bView)
        return;

    // 살짝 어둡게
    SolidBrush diminishBrush(Color(190, 0, 0, 0));
    pGraphics->FillRectangle(&diminishBrush, 0, 0, WINCX, WINCY);

    Image* pImg(nullptr);
    VEC vCellSize{};
    VEC vImgSize{};

    // 세피로트 그리기 
    if (m_eCurState == ITEM_SELECT_UI_STATE::READY
        || m_eCurState == ITEM_SELECT_UI_STATE::BROKEN)
    {
        pImg = CImgMgr::GetInstance()->FindImg(L"Sephirite_Broken");
        if (nullptr == pImg)
            return;
        vCellSize = { 96.f, 96.f };
        vImgSize = vCellSize * m_fUIScale;

        // 중앙에서 좀 내리기
        RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
                        m_tInfo.vPoint.fY - vImgSize.fY * 0.25f,
                        vImgSize.fX,
                        vImgSize.fY };

        pGraphics->DrawImage(
            pImg, rcDest,
            vCellSize.fX * m_tFrame.iStart,
            vCellSize.fY * m_tFrame.iMotion,
            vCellSize.fX,
            vCellSize.fY,
            UnitPixel
        );
    }

    // 부수기 안내 렌더링
    if (m_eCurState == ITEM_SELECT_UI_STATE::READY)
    {
        wstring strBroken = L"눌러서 부수기";

        pImg = CImgMgr::GetInstance()->FindImg(L"KeyUI");
        if (nullptr == pImg)
            return;

        float fKeyImgWidth = 10.f * m_fUIScale;
        float fTextWidth = 40.f * m_fUIScale;
        float fGap = 3.f * m_fUIScale;
        float fAllWidth = fKeyImgWidth + fTextWidth + fGap;
        float fHeight = 7.f * m_fUIScale;

        RectF rcBroken = { m_tInfo.vPoint.fX - fAllWidth * 0.5f, m_tInfo.vPoint.fY + vImgSize.fY * 0.6f,
                            fAllWidth, fHeight };

        // rcLevelUp 안에 포함되는 위치
        RectF rcKeyImg = rcBroken;
        rcKeyImg.Width = fKeyImgWidth;

        vCellSize = { 13.f, 9.f };
        pGraphics->DrawImage(
            pImg, rcKeyImg,
            9 * vCellSize.fX,
            6 * vCellSize.fY,
            vCellSize.fX,
            vCellSize.fY,
            UnitPixel
        );

        RectF rcBgStr = rcBroken;
        rcBgStr.X += 2.f;
        rcBgStr.Y += 2.f;
        CFontMgr::GetInstance()->DrawString(pGraphics, strBroken, FONT_TYPE::NORMAL, rcBgStr, Color{ 255, 0, 0, 0 }, 28.f, StringAlignmentFar);
        CFontMgr::GetInstance()->DrawString(pGraphics, strBroken, FONT_TYPE::NORMAL, rcBroken, Color{ 255, 255, 255, 255 }, 28.f, StringAlignmentFar);
    }

    for (auto& itemSlot : m_vecItemSlot)
        itemSlot->Render(pGraphics);
}

void CItemSelectUI::Release()
{
    for_each(m_vecItemSlot.begin(), m_vecItemSlot.end(), SafeDelete<CItemSelectSlotUI*>);
    m_vecItemSlot.clear();
    m_vecItemSlot.shrink_to_fit();

    if (nullptr != m_pRerollBtn)
    {
        m_pRerollBtn->SetDead(true);
        m_pRerollBtn = nullptr;
    }
    if (nullptr != m_pSkipBtn)
    {
        m_pSkipBtn->SetDead(true);
        m_pSkipBtn = nullptr;
    }
}

void CItemSelectUI::Show()
{
    m_bView = true;

    Initialize();
    GachaItems();
    CreateBtn();
}

void CItemSelectUI::Hide()
{
    m_bView = false;

    if (nullptr != m_pRerollBtn)
        m_pRerollBtn->Hide();
    if (nullptr != m_pSkipBtn)
        m_pSkipBtn->Hide();
}

void CItemSelectUI::Toggle()
{
    m_bView = !m_bView;

    Initialize();
    GachaItems();
    CreateBtn();
}

void CItemSelectUI::UpdateTime()
{
    m_dStateTime += DT;

    switch (m_eCurState)
    {
    case ITEM_SELECT_UI_STATE::BROKEN:
        if (m_dStateTime >= m_dBrokenTime)
        {
            RequestChange(ITEM_SELECT_UI_STATE::MOVE);
        }
        else if (m_dStateTime >= m_dSlotRenderTime)
        {
            for (auto& itemSlot : m_vecItemSlot)
            {
                if (false == itemSlot->GetView())
                {
                    itemSlot->Show();
                    CSoundMgr::GetInstance()->PlaySound(L"SephirateSelect.wav", CHANNEL_GROUPID::SFX, 1.f);
                    itemSlot->SetSpread(true);
                }
            }
        }
        break;
    }
}

void CItemSelectUI::Move()
{
    if (ITEM_SELECT_UI_STATE::MOVE != m_eCurState)
        return;

    VEC vDir = { -1.f, 0.f };
    m_tInfo.vPoint += vDir * m_fSpeed * DT;

    if (m_tInfo.vPoint.fX <= 300.f)
    {
        m_tInfo.vPoint.fX = 300.f;
        m_fSpeed = 0.f;

        RequestChange(ITEM_SELECT_UI_STATE::SELECT);
    }
}

void CItemSelectUI::HandleCollisionMouse()
{
    if (m_eCurState != ITEM_SELECT_UI_STATE::SELECT)
        return;
    // 마우스 포인터와 충돌하는 슬롯이 없는 경우에는 -1을 반환해줌
    int iMouseHoverSlot = CCollisionMgr::GetCollisionSlotIndex<CItemSelectSlotUI>(m_pMouse->GetInfo().vPoint, m_vecItemSlot);
    for (int i = 0; i < m_vecItemSlot.size(); ++i)
    {
        bool bCol = (i == iMouseHoverSlot && -1 != m_vecItemSlot[i]->GetItemID());
        m_vecItemSlot[i]->SetCollide(bCol);
    }

    if (-1 != iMouseHoverSlot)
    {
        m_pMouse->SetHoverReferItem({ m_vecItemSlot[iMouseHoverSlot]->GetItemID(), ITEM_SOURCE::SELECT });
        CUIMgr::GetInstance()->ShowUI(UIID::ITEM_TOOLTIP);

        VEC vPoint = m_vecItemSlot[iMouseHoverSlot]->GetInfo().vPoint;
        VEC vOffset = m_vecItemSlot[iMouseHoverSlot]->GetInfo().vSize;
        vPoint += VEC{ vOffset.fX * 0.5f, -vOffset.fY };
        CUIMgr::GetInstance()->SetPos(UIID::ITEM_TOOLTIP, vPoint);
    }
    else if (m_pMouse->GetHoverReferItem().eItemSource == ITEM_SOURCE::SELECT)
    {
        CUIMgr::GetInstance()->HideUI(UIID::ITEM_TOOLTIP);
        m_pMouse->SetHoverReferItem({ -1, ITEM_SOURCE::END });
    }

    // 드래그 시작
    if (KEY_DOWN(VK_LBUTTON) && -1 != iMouseHoverSlot)
    {
        // 마우스가 현재 드래그 중인 아이템을 참조하게 해줌 - 렌더용
        m_pMouse->SetDragReferItem({ m_vecItemSlot[iMouseHoverSlot]->GetItemID(), ITEM_SOURCE::SELECT });
    }
}

void CItemSelectUI::GachaItems()
{
    for (size_t i = 0; i < m_vecItemSlot.size(); ++i)
    {
        auto& itemSlot = m_vecItemSlot[i];

        if (itemSlot == nullptr)
        {
            itemSlot = static_cast<CItemSelectSlotUI*>(CAbstractFactory<CItemSelectSlotUI>::CreateUI(m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, m_pMouse));
            itemSlot->SetTarget(this);
            itemSlot->SetDistance(150.f);

            // 위부터 시작(-90도) + 한 칸이 차지하는 각도(72도 - 아이템 5개 기준) 
            float fRadian = -PI * 0.5f + 2.f * PI * static_cast<float>(i) / static_cast<float>(m_vecItemSlot.size());

            // 단위원 1 기준이라 이미 Normalize 상태임
            itemSlot->SetDirVector({ cosf(fRadian), sinf(fRadian) });
        }

        // 다시 뽑기 시 다시 가운데로 모으고 펼칠 수 있게하고 일단 숨김
        itemSlot->SetPos(m_tInfo.vPoint);
        itemSlot->SetSpread(false);
        itemSlot->Hide();
        itemSlot->SetItemID(uniform_int_distribution<int>(0, 8)(g_engine));
    }
}

void CItemSelectUI::ReloadItems()
{
    for (size_t i = 0; i < m_vecItemSlot.size(); ++i)
    {
        auto& itemSlot = m_vecItemSlot[i];
        if (nullptr != itemSlot)
            itemSlot->SetItemID(uniform_int_distribution<int>(0, 8)(g_engine));
    }
}

void CItemSelectUI::CreateBtn()
{
    CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (nullptr == pPlayer)
        return;
    int iDice = pPlayer->GetDice();

    // 버튼 정보 초기화
    if (nullptr == m_pRerollBtn)
    {
        RectF rcRerollBtn = { 210.f, WINCY - 175.f, 190.f, 70.f };
        wstring strRerollBtn = L"리롤 (" + to_wstring(iDice) + L"회)";
        m_pRerollBtn = CAbstractFactory<CButton>::CreateButton(m_pMouse, rcRerollBtn, strRerollBtn);
        m_pRerollBtn->SetOnClick([this, pPlayer]() {
            this->ReloadItems();
            if (nullptr == pPlayer)
                return;

            pPlayer->AddDice(-1);
            int iDice = pPlayer->GetDice();
            if (iDice <= 0)
                this->m_pRerollBtn->SetEnable(false);

            if (nullptr == this->m_pRerollBtn)
                return;

            CSoundMgr::GetInstance()->PlaySound(L"Reroll.wav", CHANNEL_GROUPID::SFX, 1.f);

            m_pRerollBtn->SetString(L"리롤 (" + to_wstring(iDice) + L"회)");
            });
        CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pRerollBtn);
    }
    if (nullptr == m_pSkipBtn)
    {
        RectF rcSkipBtn = { 255.f, WINCY - 100.f, 100.f, 50.f };
        wstring strSkipBtn = L"스킵";
        m_pSkipBtn = CAbstractFactory<CButton>::CreateButton(m_pMouse, rcSkipBtn, strSkipBtn, L"BlueButton");
        m_pSkipBtn->SetOnClick([this]() {
            this->Hide();
            CUIMgr::GetInstance()->HideUI(UIID::INVENTORY);
            });
        CObjMgr::GetInstance()->AddObject(OBJID::UI, m_pSkipBtn);
    }

    // 일단 숨기고, 
    m_pRerollBtn->Hide();
    m_pRerollBtn->SetEnable(iDice >= 1);
    m_pSkipBtn->Hide();
}

void CItemSelectUI::KeyInput()
{
    if (KEY_DOWN(VK_SPACE) && ITEM_SELECT_UI_STATE::READY == m_eCurState)
    {
        RequestChange(ITEM_SELECT_UI_STATE::BROKEN);
    }
}

void CItemSelectUI::ApplyChange()
{
    if (m_eCurState != m_eNextState)
    {
        switch (m_eNextState)
        {
        case ITEM_SELECT_UI_STATE::READY:
            SetFrame(0, 15, 0, 0.08);
            CSoundMgr::GetInstance()->PlaySound(L"SephiriteOpen.wav", CHANNEL_GROUPID::SFX, 1.f);
            break;
        case ITEM_SELECT_UI_STATE::BROKEN:
            SetFrame(0, 79, 0, m_dBrokenTime / 79);
            break;
        case ITEM_SELECT_UI_STATE::MOVE:
            m_fSpeed = 300.f;
            break;
        case ITEM_SELECT_UI_STATE::SELECT:  // 레벨업으로 들어온건 바로 여기로
            m_tInfo.vPoint.fX = 300.f;      
            for (auto& itemSlot : m_vecItemSlot)
            {
                itemSlot->SetPos(m_vPrePoint + VEC{ itemSlot->GetDirVector() * itemSlot->GetDistance() });
                itemSlot->Show();
            }

            CUIMgr::GetInstance()->ShowUI(UIID::INVENTORY);
            if (nullptr != m_pRerollBtn)
                m_pRerollBtn->Show();
            if (nullptr != m_pSkipBtn)
                m_pSkipBtn->Show();

            break;
        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}
