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
    constexpr int EXPOSED_HIT_COUNT = 1;
}

CErma::CErma()
    : CState(ERMA_STATE::END, ERMA_STATE::IDLE),
    m_vAnchor{},
    m_vAirStart{},
    m_vAirTarget{},
    m_vReturnStart{},
    m_dStateElapseTime(0.),
    m_iExposureHitCount(0),
    m_bHasAnchor(false),
    m_bManagedByGolem(false),
    m_bExposed(true),
    m_bVisible(true),
    m_bReturnComplete(false),
    m_bFinalStone(false),
    m_bDefeated(false),
    m_bRemoveRequested(false)
{
}

CErma::~CErma()
{
}

void CErma::Initialize()
{
    m_tInfo = { WINCX >> 1, WINCY >> 1, 100.f, 100.f };
    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 3;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Erma/Erma_LEFT.png",
        L"Erma_L");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Erma/Erma_RIGHT.png",
        L"Erma_R");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_LEFT.png",
        L"Erma_Stone_L");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Erma/Erma_GoToStone_RIGHT.png",
        L"Erma_Stone_R");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Erma/Erma_Stone_Shiny.png",
        L"Erma_Stone_Shiny");

    m_pFrameKey = L"Erma_L";
    SetFrame(0, 5, 0, 0.2);

    m_dHitElapseTime = 0.;
    m_dIframeTime = 0.3;
    m_bHit = false;

    m_tStat = { 200, 200, 30 };
}

int CErma::Update()
{
    if (m_bDead || m_bRemoveRequested)
        return DEAD;

    UpdateTime();
    ApplyChange();
    Move();

    __super::UpdateRect();
    UpdateFrame();
    return NOEVENT;
}

void CErma::LateUpdate()
{
}

void CErma::Render(Graphics* pGraphics)
{
    if (!m_bVisible)
        return;

    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

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

    VEC vImgSize = vCellSize * PIXEL_SCALE;
    RectF rcDest{
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

void CErma::UpdateFrame()
{
    if (m_eCurState == ERMA_STATE::RETURN)
        return;

    if (m_tFrame.dFrameSpeed <= CTimeMgr::GetInstance()->GetTime(m_tFrame.iFrameTime))
    {
        ++m_tFrame.iStart;
        QueryPerformanceCounter(&m_tFrame.iFrameTime);

        if (m_tFrame.iStart > m_tFrame.iEnd)
        {
            if (m_eCurState == ERMA_STATE::STONE)
            {
                m_tFrame.iStart = m_tFrame.iEnd;

                if (m_bManagedByGolem)
                    m_eNextState = ERMA_STATE::RETURN;
                else
                    m_bDead = true;
            }
            else
            {
                m_tFrame.iStart = 0;
            }
        }
    }
}

void CErma::ApplyChange()
{
    if (m_eCurState == m_eNextState)
        return;

    m_eCurState = m_eNextState;
    m_dStateElapseTime = 0.;

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
        m_tInfo.vPoint = m_vAirStart;
        break;

    case ERMA_STATE::STUN:
        m_pFrameKey = L"Erma_L";
        SetFrame(0, 5, 3, 0.2);
        m_tInfo.vPoint = m_vAirTarget;
        break;

    case ERMA_STATE::STONE:
        m_pFrameKey = L"Erma_Stone_L";
        SetFrame(0, 18, 0, 0.1);
        break;

    case ERMA_STATE::RETURN:
        m_pFrameKey = L"Erma_Stone_L";
        SetFrame(18, 18, 0, 10.);
        m_vReturnStart = m_tInfo.vPoint;
        break;

    case ERMA_STATE::LIGHT:
        m_pFrameKey = L"Erma_Stone_Shiny";
        SetFrame(0, 2, 0, 0.2);
        break;

    default:
        break;
    }
}

void CErma::SetDamage(int iDamage, CObj* pObj)
{
    if (m_bDefeated || m_bHit)
        return;

    if (m_bManagedByGolem)
    {
        if (!m_bExposed || m_eCurState != ERMA_STATE::STUN)
            return;

        m_bHit = true;
        m_tStat.iHp = std::clamp(m_tStat.iHp - iDamage, 0, m_tStat.iMaxHp);
        ++m_iExposureHitCount;

        if (m_iExposureHitCount >= EXPOSED_HIT_COUNT)
        {
            m_bFinalStone = (m_tStat.iHp <= 0);
            m_eNextState = ERMA_STATE::STONE;
        }
        return;
    }

    m_bHit = true;
    m_tStat.iHp -= iDamage;
    m_eNextState = ERMA_STATE::STUN;

    if (m_tStat.iHp <= 0)
    {
        m_tStat.iHp = 0;
        m_eNextState = ERMA_STATE::STONE;
    }
}

void CErma::UpdateTime()
{
    m_dStateElapseTime += DT;

    if (m_bHit)
    {
        m_dHitElapseTime += DT;
        if (m_dHitElapseTime >= m_dIframeTime)
        {
            m_bHit = false;
            m_dHitElapseTime = 0.;
        }
    }

    if (m_eCurState == ERMA_STATE::AIR && m_dStateElapseTime >= AIR_TIME)
    {
        m_eNextState = ERMA_STATE::STUN;
    }
    else if (m_eCurState == ERMA_STATE::RETURN &&
        m_dStateElapseTime >= RETURN_TIME)
    {
        m_tInfo.vPoint = m_vAnchor;
        m_bExposed = false;
        m_bReturnComplete = true;

        if (m_bFinalStone)
            m_bDefeated = true;

        m_bVisible = false;

        m_eNextState = ERMA_STATE::IDLE;
    }
}

void CErma::Move()
{
    if (m_eCurState == ERMA_STATE::AIR)
    {
        float fRatio = static_cast<float>(m_dStateElapseTime / AIR_TIME);
        fRatio = std::clamp(fRatio, 0.f, 1.f);

        m_tInfo.vPoint = m_vAirStart + (m_vAirTarget - m_vAirStart) * fRatio;
        m_tInfo.vPoint.fY -= sinf(PI * fRatio) * AIR_ARC_HEIGHT;
    }
    else if (m_eCurState == ERMA_STATE::RETURN)
    {
        float fRatio = static_cast<float>(m_dStateElapseTime / RETURN_TIME);
        fRatio = std::clamp(fRatio, 0.f, 1.f);
        const float fSmooth = fRatio * fRatio * (3.f - 2.f * fRatio);
        m_tInfo.vPoint = m_vReturnStart + (m_vAnchor - m_vReturnStart) * fSmooth;
    }
    else if (m_bManagedByGolem && m_bHasAnchor && !m_bVisible)
    {
        m_tInfo.vPoint = m_vAnchor;
    }
}

void CErma::SetAnchor(const VEC& vAnchor)
{
    m_vAnchor = vAnchor;
    m_bHasAnchor = true;

    if (m_bManagedByGolem && !m_bVisible)
        m_tInfo.vPoint = m_vAnchor;
}

void CErma::SetExposed(bool bExposed)
{
    m_bExposed = bExposed;

    if (!m_bManagedByGolem || m_bDefeated)
        return;

    if (!m_bExposed)
    {
        m_bVisible = false;
        m_bHit = false;
        m_dHitElapseTime = 0.;
        m_eNextState = ERMA_STATE::IDLE;
    }
}

void CErma::StartExposure(const VEC& vHeadPoint, const VEC& vStunPoint)
{
    if (!m_bManagedByGolem || m_bDefeated)
        return;

    m_vAnchor = vHeadPoint;
    m_vAirStart = vHeadPoint;
    m_vAirTarget = vStunPoint;
    m_iExposureHitCount = 0;
    m_bReturnComplete = false;
    m_bFinalStone = false;
    m_bExposed = true;
    m_bVisible = true;
    m_bHit = false;
    m_dHitElapseTime = 0.;
    m_tInfo.vPoint = m_vAirStart;
    m_eNextState = ERMA_STATE::AIR;
}
