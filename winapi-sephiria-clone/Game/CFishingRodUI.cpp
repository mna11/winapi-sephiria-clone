#include "pch.h"
#include "CFishingRodUI.h"

#include "CPlayer.h"

#include "CObjMgr.h"
#include "CImgMgr.h"
#include "CUIMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

CFishingRodUI::CFishingRodUI()
    : CState<FISHING_ROD_STATE>(FISHING_ROD_STATE::END,FISHING_ROD_STATE::IDLE_OUT_WATER),
    m_bLoop(false),m_bAnimationFinished(false)
{
}

CFishingRodUI::~CFishingRodUI()
{
    Release();
}

void CFishingRodUI::Initialize()
{
    m_pFrameKey = L"Fishing_Rod";

    m_fUIScale = PIXEL_SCALE * 1.5f;
    m_tInfo.vSize = VEC{ 64.f, 64.f } * m_fUIScale;
    m_tInfo.vPoint = { 0, WINCY - m_tInfo.vSize.fY };

    m_eRender = RENDERID::UI;
    m_iRenderLayer = 0;

    m_bView = false;
}

int CFishingRodUI::Update()
{
    if (m_bDead)
        return DEAD;

    if (!m_bView)
        return NOEVENT;

    ApplyChange();
    UpdateTime();

    UpdateFrame();
    __super::UpdateRect();
    return NOEVENT;
}

void CFishingRodUI::LateUpdate()
{
}

void CFishingRodUI::Render(Graphics* pGraphics)
{
    if (!m_bView || m_bDead)
        return;

    Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
    if (nullptr == pImg)
        return;

    VEC vCellSize = m_tInfo.vSize / m_fUIScale;
    RectF DestRect { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, m_tInfo.vSize.fX, m_tInfo.vSize.fY};

#ifdef _DEBUG
    SolidBrush WhiteBrush(Color(255, 255, 255, 255));
    pGraphics->FillRectangle(&WhiteBrush, DestRect);
#endif // _DEBUG

    pGraphics->DrawImage(
        pImg, DestRect,
        m_tFrame.iStart * vCellSize.fX,
        m_tFrame.iMotion * vCellSize.fY,
        vCellSize.fX,
        vCellSize.fY,
        UnitPixel
    );
}

void CFishingRodUI::Release()
{
}

void CFishingRodUI::Show()
{
    // 이미 표시 중이면 애니메이션을 다시 시작하지 않음
    if (m_bView)
        return;

    CUI::Show();
    m_eCurState = FISHING_ROD_STATE::END;
    m_eNextState = FISHING_ROD_STATE::IDLE_OUT_WATER;

    ApplyChange();
}

void CFishingRodUI::ApplyChange()
{
    if (m_eCurState == m_eNextState)
        return;

    if (m_eNextState == FISHING_ROD_STATE::END)
        return;

    m_bLoop = false;
    m_bAnimationFinished = false;

    switch (m_eNextState)
    {
    case FISHING_ROD_STATE::PREPARE_CAST:
        SetFrame(0, 5, 0, 0.1);
        break;

    case FISHING_ROD_STATE::CASTING:
        SetFrame(0, 11, 1, 0.08);
        break;

    case FISHING_ROD_STATE::IDLE_OUT_WATER:
        SetFrame(0, 3, 2, 0.15);
        m_bLoop = true;
        break;

    case FISHING_ROD_STATE::IDLE_IN_WATER:
        SetFrame(0, 3, 3, 0.15);
        m_bLoop = true;
        break;

    case FISHING_ROD_STATE::HOOK_SET:
        SetFrame(0, 4, 4, 0.08);
        break;

    case FISHING_ROD_STATE::REELING:
        SetFrame(0, 4, 5, 0.1);
        m_bLoop = true;
        break;

    case FISHING_ROD_STATE::CAUGHT:
        SetFrame(0, 2, 6, 0.12);
        break;

    default:
        return;
    }

    m_eCurState = m_eNextState;
}

void CFishingRodUI::UpdateTime()
{
    switch (m_eCurState)
    {
    case FISHING_ROD_STATE::PREPARE_CAST:
        if (m_bAnimationFinished)
            RequestChange(FISHING_ROD_STATE::CASTING);
        break;
    case FISHING_ROD_STATE::CASTING:
        if (m_bAnimationFinished)
            RequestChange(FISHING_ROD_STATE::IDLE_OUT_WATER);
        break;
    default:
        break;
    }
}

void CFishingRodUI::UpdateFrame()
{
    if (m_bAnimationFinished || m_tFrame.dFrameSpeed <= 0.)
        return;

    m_tFrame.dFrameElapsedTime += DT;
    if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
    {
        ++m_tFrame.iStart;
        m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

        if (m_tFrame.iStart > m_tFrame.iEnd)
        {
            if (m_bLoop)
                m_tFrame.iStart = 0;
            else
            {
                m_tFrame.iStart = m_tFrame.iEnd;
                m_bAnimationFinished = true;
            }
        }
    }
}