#include "pch.h"
#include "CErma.h"

#include "CGolem.h"
#include "CScene.h"
#include "CBossStage.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"
#include "CSceneMgr.h"

#include "CSoundMgr.h"
#include "CEffectMgr.h"

CErma::CErma()
    : CState(ERMA_STATE::END, ERMA_STATE::IDLE),
    m_dStateTime(0.),
    m_pOwner(nullptr),
    m_bView(true)
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

    m_pFrameKey = L"Erma_R";
    SetFrame(0, 5, 0, 0.2);

    m_tStat = { 100, 100, 0 };

    m_iLeaf = 0;
    m_iExp = 0;
}

int CErma::Update()
{
    if (m_bDead)
        return DEAD;

    if (!m_bView)
        return NOEVENT;

    ApplyChange();
    UpdateTime();
    Move();

    UpdateFrame();
    __super::UpdateRect();
    return NOEVENT;
}

void CErma::LateUpdate()
{
    if (!m_bView)
        return;

    if (m_eCurState == ERMA_STATE::STUN && m_tStat.iHp <= 0)
    {
        if (nullptr == m_pOwner)
            return;

        if (0 == m_pOwner->GetPhase())
            StartReturn();
        else
        {
            RequestChange(ERMA_STATE::IDLE);
            static_cast<CBossStage*>(CSceneMgr::GetInstance()->GetCurrentScene())->RequestChange(BOSS_STAGE_STATE::FINAL);
        }
    }
}

void CErma::Render(Graphics* pGraphics)
{
    if (!m_bView)
        return;

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

    VEC vImgSize = vCellSize * PIXEL_SCALE;

    float fOffset(0.f);
    if (m_bHit && m_dIframeTime > 0.)
    {
        float fRatio = clamp(static_cast<float>(m_dHitElapseTime / m_dIframeTime), 0.f, 1.f);
        float fAmplitude = vCellSize.fY * 0.5f * PIXEL_SCALE;
        float fDecay = (1.f - fRatio) * (1.f - fRatio);

        fOffset = -fAmplitude * sinf(fRatio * PI) * fDecay;
    }

    RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vScroll.fX - fOffset * 0.5f, m_tInfo.vPoint.fY - vImgSize.fY * 0.5f + vScroll.fY + fOffset, vImgSize.fX + fOffset, vImgSize.fY - fOffset };

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
        switch (m_eNextState)
        {
        case ERMA_STATE::IDLE:
            m_pFrameKey = L"Erma_R";
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
            CSoundMgr::GetInstance()->PlaySound(L"GolemSlotTransform.wav", CHANNEL_GROUPID::SFX, 1.f);
            m_pFrameKey = L"Erma_Stone_L";
            SetFrame(0, 18, 0, 0.08);
            break;
        case ERMA_STATE::RETURN:
            m_pFrameKey = L"Erma_Stone_L";
            SetFrame(18, 18, 0, 10.);
            break;
        case ERMA_STATE::LIGHT:
            m_pFrameKey = L"Erma_Stone_Shiny";
            SetFrame(0, 2, 0, 0.2);
            break;
        case ERMA_STATE::DOWN:
            m_pFrameKey = L"Erma_L";
            SetFrame(0, 3, 4, 0.2);
            break;
        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}

void CErma::UpdateFrame()
{
    if (m_eCurState == ERMA_STATE::RETURN)
        return;

    m_tFrame.dFrameElapsedTime += DT;
    if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
    {
        ++m_tFrame.iStart;
        m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

        if (m_tFrame.iStart > m_tFrame.iEnd)
        {
            if (m_eCurState == ERMA_STATE::STONE)
            {
                m_tFrame.iStart = m_tFrame.iEnd;
                RequestChange(ERMA_STATE::RETURN);
            }
            else if (m_eCurState == ERMA_STATE::DOWN)
            {
                m_tFrame.iStart = m_tFrame.iEnd;
            }
            else
            {
                m_tFrame.iStart = 0;
            }
        }
    }
}

void CErma::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
    if (!m_bView || m_eCurState != ERMA_STATE::STUN)
        return;
    
    if (!ApplyDamage(iDamage, eHit))
        return;
}

void CErma::StartReturn()
{
    if (nullptr == m_pOwner)
        return;

    m_vReturnPoint = m_pOwner->GetEnterPoint();

    RequestChange(ERMA_STATE::STONE);
}

void CErma::StartExpose(VEC vPoint)
{
    m_vExposePoint = vPoint;
    m_bView = true;

    m_tStat.iMaxHp = 100;
    m_tStat.iHp = m_tStat.iMaxHp;

    RequestChange(ERMA_STATE::AIR);
}

bool CErma::IsReturnFinish()
{
    if (nullptr == m_pOwner)
        return false;

    return ((m_tInfo.vPoint - m_pOwner->GetEnterPoint()).Norm() == 0.f);
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
    switch (m_eCurState)
    {
    case ERMA_STATE::RETURN:
    {
        VEC vDistance = m_vReturnPoint - m_tInfo.vPoint;
        float fDistance = vDistance.Norm();
        float fMoveDistance = 200.f * DT;
        if (fDistance <= fMoveDistance)
        {
            m_tInfo.vPoint = m_vReturnPoint;
            m_bView = false;

            CSoundMgr::GetInstance()->PlaySound(L"GolemEnter.wav", CHANNEL_GROUPID::SFX, 1.f);

            if (nullptr != m_pOwner)
                m_pOwner->RequestChange(GOLEM_STATE::RECOVER);
        }
        else
            m_tInfo.vPoint += vDistance.Normalize() * fMoveDistance;

        break;
    }
    case ERMA_STATE::AIR:
    {
        VEC vDistance = m_vExposePoint - m_tInfo.vPoint;
        float fDistance = vDistance.Norm();
        float fMoveDistance = 400.f * DT;
        if (fDistance <= fMoveDistance)
        {
            m_tInfo.vPoint = m_vExposePoint;

            if (nullptr != m_pOwner)
            {
                RequestChange(ERMA_STATE::STUN);
            }

        }
        else
            m_tInfo.vPoint += vDistance.Normalize() * fMoveDistance;

        break;
    }
    }

}

void CErma::Attack()
{
}

void CErma::Down()
{
    m_eNextState = ERMA_STATE::DOWN;
}
