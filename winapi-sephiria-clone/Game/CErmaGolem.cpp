#include "pch.h"
#include "CErmaGolem.h"

#include "CErma.h"
#include "CErmaHand.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"
#include "CFontMgr.h"

namespace
{
    constexpr float ARENA_LEFT = 3350.f;
    constexpr float ARENA_RIGHT = 4570.f;
    constexpr float ARENA_TOP = 1080.f;
    constexpr float ARENA_BOTTOM = 2300.f;
    constexpr float HEAD_OFFSET_Y = -100.f;
    constexpr float STUN_OFFSET_Y = 350.f;
}

CErmaGolem::CErmaGolem()
    : CState(ERMA_GOLEM_STATE::END, ERMA_GOLEM_STATE::WAIT),
    m_pErma(nullptr),
    m_pLeftHand(nullptr),
    m_pRightHand(nullptr),
    m_dStateElapseTime(0.),
    m_bNextLeftHand(false),
    m_bBattleStarted(false),
    m_bEnding(false)
{
}

CErmaGolem::~CErmaGolem()
{
    Release();
}

void CErmaGolem::Initialize()
{
    m_tInfo = { 0.f, 0.f, 260.f, 220.f };
    m_tStat = { 600, 600, 30 };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 2;

    m_dIframeTime = 0.16;
    m_dHitElapseTime = 0.;
    m_bHit = false;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Body/BigGolem_Body.png",
        L"Erma_Golem_Body");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Effect/Head/BigGolem_Head.png",
        L"Erma_Golem_Head");
}

int CErmaGolem::Update()
{
    if (m_bDead || m_bEnding)
        return DEAD;

    CheckBattleStart();
    UpdateTime();
    ApplyChange();

    if (m_pErma != nullptr && m_pErma->IsDefeated())
        BeginEnding();

    __super::UpdateRect();
    return NOEVENT;
}

void CErmaGolem::LateUpdate()
{
}

void CErmaGolem::Render(Graphics* pGraphics)
{
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
    ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;

    Image* pBody = CImgMgr::GetInstance()->FindImg(L"Erma_Golem_Body");
    if (pBody != nullptr)
    {
        constexpr float fCellX = 113.f;
        constexpr float fCellY = 55.f;
        const int iFrame = (m_eCurState == ERMA_GOLEM_STATE::EXPOSED) ? 1 : 0;
        VEC vDrawSize = VEC{ fCellX, fCellY } *PIXEL_SCALE;
        RectF rcDest{
            m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
            m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
            vDrawSize.fX,
            vDrawSize.fY
        };

        pGraphics->DrawImage(
            pBody, rcDest,
            fCellX * iFrame, 0.f,
            fCellX, fCellY,
            UnitPixel, pImgAttr);
    }

    Image* pHead = CImgMgr::GetInstance()->FindImg(L"Erma_Golem_Head");
    if (pHead != nullptr)
    {
        constexpr float fCellX = 93.f;
        constexpr float fCellY = 80.f;
        const int iFrameY = (m_eCurState == ERMA_GOLEM_STATE::EXPOSED) ? 2 : 0;
        VEC vDrawSize = VEC{ fCellX, fCellY } *PIXEL_SCALE;
        RectF rcDest{
            m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
            m_tInfo.vPoint.fY + HEAD_OFFSET_Y - vDrawSize.fY * 0.5f + vScroll.fY,
            vDrawSize.fX,
            vDrawSize.fY
        };

        pGraphics->DrawImage(
            pHead, rcDest,
            0.f, fCellY * iFrameY,
            fCellX, fCellY,
            UnitPixel, pImgAttr);
    }

#ifdef _DEBUG
    Pen pen(Color(255, 80, 200, 255), 2.f);
    pGraphics->DrawRectangle(
        &pen,
        static_cast<int>(m_tRect.left + vScroll.fX),
        static_cast<int>(m_tRect.top + vScroll.fY),
        static_cast<int>(m_tInfo.vSize.fX),
        static_cast<int>(m_tInfo.vSize.fY));
#endif
}

void CErmaGolem::Release()
{
}

void CErmaGolem::ApplyChange()
{
    if (m_eCurState == m_eNextState)
        return;

    m_eCurState = m_eNextState;
    m_dStateElapseTime = 0.;

    switch (m_eCurState)
    {
    case ERMA_GOLEM_STATE::INTRO:
        m_bBattleStarted = true;
        break;

    case ERMA_GOLEM_STATE::LEFT_HAND_ATTACK:
        if (m_pLeftHand != nullptr && m_pTarget != nullptr)
            m_pLeftHand->StartSlam(m_pTarget->GetInfo().vPoint);
        break;

    case ERMA_GOLEM_STATE::RIGHT_HAND_ATTACK:
        if (m_pRightHand != nullptr && m_pTarget != nullptr)
            m_pRightHand->StartSlam(m_pTarget->GetInfo().vPoint);
        break;

    case ERMA_GOLEM_STATE::EXPOSED:
        if (m_pLeftHand != nullptr)
            m_pLeftHand->ForceBreak();
        if (m_pRightHand != nullptr)
            m_pRightHand->ForceBreak();
        if (m_pErma != nullptr)
        {
            const VEC vHeadPoint = m_tInfo.vPoint + VEC{ 0.f, HEAD_OFFSET_Y };
            const VEC vStunPoint = m_tInfo.vPoint + VEC{ 0.f, STUN_OFFSET_Y };
            m_pErma->StartExposure(vHeadPoint, vStunPoint);
        }
        break;

    case ERMA_GOLEM_STATE::RECOVER:
        m_tStat.iHp = m_tStat.iMaxHp;
        if (m_pErma != nullptr)
        {
            m_pErma->SetExposed(false);
            m_pErma->SetVisible(false);
        }
        if (m_pLeftHand != nullptr)
            m_pLeftHand->Restore();
        if (m_pRightHand != nullptr)
            m_pRightHand->Restore();
        break;

    default:
        break;
    }
}

void CErmaGolem::SetDamage(int iDamage, CObj* pObj)
{
    AddStaggerDamage(iDamage);
}

void CErmaGolem::AddStaggerDamage(int iDamage)
{
    if (!m_bBattleStarted ||
        m_eCurState == ERMA_GOLEM_STATE::INTRO ||
        m_eCurState == ERMA_GOLEM_STATE::EXPOSED ||
        m_eCurState == ERMA_GOLEM_STATE::RECOVER ||
        m_bHit)
    {
        return;
    }

    m_bHit = true;
    m_tStat.iHp -= iDamage;

    if (m_tStat.iHp <= 0)
    {
        m_tStat.iHp = 0;
        m_eNextState = ERMA_GOLEM_STATE::EXPOSED;
    }
}

void CErmaGolem::SetParts(
    CErma* pErma,
    CErmaHand* pLeftHand,
    CErmaHand* pRightHand)
{
    m_pErma = pErma;
    m_pLeftHand = pLeftHand;
    m_pRightHand = pRightHand;

    if (m_pErma != nullptr)
    {
        m_pErma->SetManagedByGolem(true);
        m_pErma->SetExposed(false);
        m_pErma->SetVisible(false);
        m_pErma->SetAnchor(m_tInfo.vPoint + VEC{ 0.f, HEAD_OFFSET_Y });
    }

    if (m_pLeftHand != nullptr)
    {
        m_pLeftHand->SetSide(ERMA_HAND_SIDE::LEFT);
        m_pLeftHand->SetOwner(this);
        m_pLeftHand->SetAnchor(m_pLeftHand->GetInfo().vPoint);
    }

    if (m_pRightHand != nullptr)
    {
        m_pRightHand->SetSide(ERMA_HAND_SIDE::RIGHT);
        m_pRightHand->SetOwner(this);
        m_pRightHand->SetAnchor(m_pRightHand->GetInfo().vPoint);
    }
}

void CErmaGolem::CheckBattleStart()
{
    if (m_eCurState != ERMA_GOLEM_STATE::WAIT || m_pTarget == nullptr)
        return;

    const VEC& vPlayerPoint = m_pTarget->GetInfo().vPoint;
    if (ARENA_LEFT <= vPlayerPoint.fX && vPlayerPoint.fX <= ARENA_RIGHT &&
        ARENA_TOP <= vPlayerPoint.fY && vPlayerPoint.fY <= ARENA_BOTTOM)
    {
        m_eNextState = ERMA_GOLEM_STATE::INTRO;
        CUIMgr::GetInstance()->ShowUI(UIID::BOSS_HP, this);
    }
}

void CErmaGolem::UpdateTime()
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

    switch (m_eCurState)
    {
    case ERMA_GOLEM_STATE::INTRO:
        if (m_dStateElapseTime >= 0.8)
            m_eNextState = ERMA_GOLEM_STATE::IDLE;
        break;

    case ERMA_GOLEM_STATE::IDLE:
        if (m_dStateElapseTime >= 0.5)
        {
            m_eNextState = m_bNextLeftHand
                ? ERMA_GOLEM_STATE::LEFT_HAND_ATTACK
                : ERMA_GOLEM_STATE::RIGHT_HAND_ATTACK;
            m_bNextLeftHand = !m_bNextLeftHand;
        }
        break;

    case ERMA_GOLEM_STATE::LEFT_HAND_ATTACK:
        if (m_dStateElapseTime >= 0.5 &&
            (m_pLeftHand == nullptr || !m_pLeftHand->IsBusy()))
        {
            m_eNextState = ERMA_GOLEM_STATE::IDLE;
        }
        break;

    case ERMA_GOLEM_STATE::RIGHT_HAND_ATTACK:
        if (m_dStateElapseTime >= 0.5 &&
            (m_pRightHand == nullptr || !m_pRightHand->IsBusy()))
        {
            m_eNextState = ERMA_GOLEM_STATE::IDLE;
        }
        break;

    case ERMA_GOLEM_STATE::EXPOSED:
        if (m_pErma == nullptr || m_pErma->IsReturnComplete())
        {
            m_eNextState = ERMA_GOLEM_STATE::RECOVER;
        }
        break;

    case ERMA_GOLEM_STATE::RECOVER:
        if (m_dStateElapseTime >= 1.)
            m_eNextState = ERMA_GOLEM_STATE::IDLE;
        break;

    default:
        break;
    }
}

void CErmaGolem::BeginEnding()
{
    if (m_bEnding)
        return;

    if (m_pErma != nullptr)
        m_pErma->RequestRemove();
    if (m_pLeftHand != nullptr)
        m_pLeftHand->RequestRemove();
    if (m_pRightHand != nullptr)
        m_pRightHand->RequestRemove();

    CUIMgr::GetInstance()->HideUI(UIID::BOSS_HP);

    m_bEnding = true;
}
