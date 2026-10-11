#include "pch.h"
#include "CErma.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"

namespace
{
    constexpr double AIR_TIME = 0.75;
    constexpr double RETURN_TIME = 1.4;
    constexpr float AIR_ARC_HEIGHT = 140.f;
    constexpr int EXPOSED_HIT_COUNT = 3;
}

CErma::CErma()
    : CState(ERMA_STATE::END, ERMA_STATE::IDLE)
{
}

CErma::~CErma()
{
    Release();
}

void CErma::Initialize()
{
    m_tInfo = { 0.f, 0.f, 100.f, 100.f };
    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 2;

    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_LEFT.png", L"Erma_L");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_RIGHT.png", L"Erma_R");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_LEFT.png", L"Erma_Stone_L");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_RIGHT.png", L"Erma_Stone_R");
    CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Monster/Boss/Erma/Erma_Stone_Shiny.png", L"Erma_Stone_Shiny");

    m_pFrameKey = L"Erma_L";
    SetFrame(0, 5, 0, 0.2);

    m_tStat = { 0, 0, 0 };
}

int CErma::Update()
{
    if (m_bDead)
        return DEAD;

    ApplyChange();
    UpdateTime();
    Move();

    __super::UpdateFrame();
    __super::UpdateRect();
    return NOEVENT;
}

void CErma::LateUpdate()
{
}

void CErma::Render(Graphics* pGraphics)
{
    const VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
    SolidBrush debugBrush(Color(255, 255, 255, 255));
    pGraphics->FillRectangle(
        &debugBrush,
        static_cast<int>(m_tRect.left + vScroll.fX),
        static_cast<int>(m_tRect.top + vScroll.fY),
        static_cast<int>(m_tInfo.vSize.fX),
        static_cast<int>(m_tInfo.vSize.fY));
#endif

    Image* pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
    if (pImg == nullptr)
        return;

    VEC vCellSize{};
    if (m_pFrameKey == L"Erma_L" || m_pFrameKey == L"Erma_R")
        vCellSize = { 31.f, 25.f };
    else if (m_pFrameKey == L"Erma_Stone_L" || m_pFrameKey == L"Erma_Stone_R")
        vCellSize = { 38.f, 37.f };
    else if (m_pFrameKey == L"Erma_Stone_Shiny")
        vCellSize = { 25.f, 25.f };

    const VEC vImgSize = vCellSize * PIXEL_SCALE;
    const RectF rcDest{
        m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX,
        m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY,
        vImgSize.fX,
        vImgSize.fY
    };

    ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;
    pGraphics->DrawImage(
        pImg, rcDest,
        vCellSize.fX * m_tFrame.iStart,
        vCellSize.fY * m_tFrame.iMotion,
        vCellSize.fX,
        vCellSize.fY,
        UnitPixel,
        pImgAttr);
}

void CErma::Release()
{
}

void CErma::ApplyChange()
{
    if (m_eCurState != m_eNextState)
    {
        switch (m_eCurState)
        {
        case ERMA_STATE::IDLE:
            m_pFrameKey = L"Erma_L";
            SetFrame(0, 5, 0, 0.1);
            break;
        case ERMA_STATE::WALK:
            m_pFrameKey = L"Erma_L";
            SetFrame(0, 7, 1, 0.1);
            break;
        case ERMA_STATE::AIR:
            m_pFrameKey = L"Erma_L";
            SetFrame(0, 7, 2, 0.08);
            break;
        case ERMA_STATE::STUN:
            m_pFrameKey = L"Erma_L";
            SetFrame(0, 5, 3, 0.2);
            break;
        case ERMA_STATE::STONE:
            m_pFrameKey = L"Erma_Stone_L";
            SetFrame(0, 18, 0, 0.1);
            break;
        case ERMA_STATE::RETURN:
            m_pFrameKey = L"Erma_Stone_L";
            SetFrame(18, 18, 0, 10.);
            break;
        case ERMA_STATE::LIGHT:
            m_pFrameKey = L"Erma_Stone_Shiny";
            SetFrame(0, 2, 0, 0.2);
            break;
        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}

void CErma::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
    if (m_bHit)
        return;

    m_bHit = true;
}

void CErma::UpdateTime()
{
    m_dStateTime += DT;

    if (m_bHit)
    {
        m_dHitElapseTime += DT;
        if (m_dHitElapseTime >= m_dIframeTime)
        {
            m_bHit = false;
            m_dHitElapseTime = 0.;
        }
    }
}

void CErma::Move()
{
}

void CErma::Attack()
{
}