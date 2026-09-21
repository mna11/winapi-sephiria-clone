#include "pch.h"
#include "CErmaHand.h"

#include "CBossErma.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"

namespace
{
    constexpr float FOLLOW_HEIGHT = 280.f;
    constexpr float FOLLOW_SPEED = 300.f;
    constexpr double READY_TIME = 2.4;
    constexpr double ALERT_START_TIME = 1.6;
    constexpr double SLAM_TIME = 0.35;
    constexpr double IMPACT_HOLD_TIME = 0.18;
    constexpr double RETURN_TIME = 0.65;
}

CErmaHand::CErmaHand()
    : CState(ERMA_HAND_STATE::END, ERMA_HAND_STATE::WAIT),
    m_eSide(ERMA_HAND_SIDE::LEFT),
    m_pOwner(nullptr),
    m_vAnchor{},
    m_vMoveStart{},
    m_vSlamTarget{},
    m_dStateElapseTime(0.),
    m_bRemoveRequested(false)
{
}

CErmaHand::~CErmaHand()
{
    Release();
}

void CErmaHand::Initialize()
{
    m_tInfo = { 0.f, 0.f, 160.f, 160.f };
    m_tStat = { 100, 100, 20 };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 3;

    m_dIframeTime = 0.3;
    m_dHitElapseTime = 0.;
    m_bHit = false;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Hand/BigGolem_Hand_LEFT.png",
        L"Erma_Golem_Hand_L");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Hand/BigGolem_Hand_RIGHT.png",
        L"Erma_Golem_Hand_R");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Effect/Boss/Alert_Square.png",
        L"Erma_Alert_Square");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Effect/Boss/Golem_Hand_Shadow_LEFT.png",
        L"Erma_Hand_Shadow_L");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Effect/Boss/Golem_Hand_Shadow_RIGHT.png",
        L"Erma_Hand_Shadow_R");
}

int CErmaHand::Update()
{
    if (m_bDead || m_bRemoveRequested)
        return DEAD;

    UpdateTime();
    ApplyChange();
    Move();

    __super::UpdateRect();
    return NOEVENT;
}

void CErmaHand::LateUpdate()
{
    if (m_eCurState != ERMA_HAND_STATE::SLAM || m_pTarget == nullptr)
        return;

    RECT rcCollision{};
    if (IntersectRect(&rcCollision, &m_tRect, &m_pTarget->GetRect()))
        m_pTarget->SetDamage(m_tStat.iAtk, this);
}

void CErmaHand::Render(Graphics* pGraphics)
{
    if (m_eCurState == ERMA_HAND_STATE::BROKEN)
        return;

    const VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
    RenderAttackEffect(pGraphics, vScroll);

    const TCHAR* pImgKey = (m_eSide == ERMA_HAND_SIDE::LEFT)
        ? L"Erma_Golem_Hand_L"
        : L"Erma_Golem_Hand_R";
    Image* pImg = CImgMgr::GetInstance()->FindImg(pImgKey);

    if (pImg != nullptr)
    {
        constexpr float CELL_X = 62.f;
        constexpr float CELL_Y = 64.f;
        const VEC vDrawSize = VEC{ CELL_X, CELL_Y } * PIXEL_SCALE;
        const RectF rcDest{
            m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
            m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
            vDrawSize.fX,
            vDrawSize.fY
        };
        ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;

        pGraphics->DrawImage(
            pImg, rcDest,
            0.f, 0.f,
            CELL_X, CELL_Y,
            UnitPixel, pImgAttr);
    }

#ifdef _DEBUG
    Pen pen(Color(255, 255, 120, 80), 2.f);
    pGraphics->DrawRectangle(
        &pen,
        static_cast<int>(m_tRect.left + vScroll.fX),
        static_cast<int>(m_tRect.top + vScroll.fY),
        static_cast<int>(m_tInfo.vSize.fX),
        static_cast<int>(m_tInfo.vSize.fY));
#endif
}

void CErmaHand::Release()
{
}

void CErmaHand::ApplyChange()
{
    if (m_eCurState == m_eNextState)
        return;

    m_eCurState = m_eNextState;
    m_dStateElapseTime = 0.;

    switch (m_eCurState)
    {
    case ERMA_HAND_STATE::WAIT:
        m_tInfo.vPoint = m_vAnchor;
        break;
    case ERMA_HAND_STATE::SLAM:
    case ERMA_HAND_STATE::RETURN:
        m_vMoveStart = m_tInfo.vPoint;
        break;
    case ERMA_HAND_STATE::BROKEN:
        m_tInfo.vSize = { 0.f, 0.f };
        break;
    default:
        break;
    }
}

void CErmaHand::SetDamage(int iDamage, CObj* pObj)
{
    if (m_eCurState == ERMA_HAND_STATE::BROKEN || m_bHit || m_pOwner == nullptr)
        return;

    if (m_pOwner->AddStaggerDamage(iDamage))
        m_bHit = true;
}

void CErmaHand::SetAnchor(const VEC& vAnchor)
{
    m_vAnchor = vAnchor;

    if (m_eCurState == ERMA_HAND_STATE::WAIT ||
        m_eCurState == ERMA_HAND_STATE::END)
    {
        m_tInfo.vPoint = m_vAnchor;
    }
}

void CErmaHand::StartSlam(const VEC& vTargetPoint)
{
    if (m_eCurState != ERMA_HAND_STATE::WAIT || IsBusy())
        return;

    m_vSlamTarget = vTargetPoint;
    m_eNextState = ERMA_HAND_STATE::READY;
}

void CErmaHand::ForceBreak()
{
    m_eNextState = ERMA_HAND_STATE::BROKEN;
}

void CErmaHand::Restore()
{
    m_tInfo.vSize = { 160.f, 160.f };
    m_bHit = false;
    m_dHitElapseTime = 0.;
    m_tInfo.vPoint = m_vAnchor;
    m_eNextState = ERMA_HAND_STATE::WAIT;
}

bool CErmaHand::IsBusy() const
{
    return m_eCurState == ERMA_HAND_STATE::READY ||
        m_eCurState == ERMA_HAND_STATE::SLAM ||
        m_eCurState == ERMA_HAND_STATE::RETURN ||
        m_eNextState == ERMA_HAND_STATE::READY ||
        m_eNextState == ERMA_HAND_STATE::SLAM ||
        m_eNextState == ERMA_HAND_STATE::RETURN;
}

void CErmaHand::UpdateTime()
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

    if (m_eCurState == ERMA_HAND_STATE::READY &&
        m_eNextState != ERMA_HAND_STATE::BROKEN &&
        m_dStateElapseTime >= READY_TIME)
    {
        m_eNextState = ERMA_HAND_STATE::SLAM;
    }
}

void CErmaHand::Move()
{
    switch (m_eCurState)
    {
    case ERMA_HAND_STATE::READY:
        if (m_pTarget != nullptr)
        {
            m_vSlamTarget = m_pTarget->GetInfo().vPoint;
            const VEC vFollowPoint{ m_vSlamTarget.fX, m_vSlamTarget.fY - FOLLOW_HEIGHT };
            const VEC vDirection = vFollowPoint - m_tInfo.vPoint;
            const float fDistance = vDirection.Norm();
            const float fMoveDistance = FOLLOW_SPEED * static_cast<float>(DT);

            if (fDistance <= fMoveDistance || fDistance <= 0.001f)
                m_tInfo.vPoint = vFollowPoint;
            else
                m_tInfo.vPoint += vDirection.Normalize() * fMoveDistance;
        }
        break;

    case ERMA_HAND_STATE::SLAM:
    {
        float fRatio = static_cast<float>(m_dStateElapseTime / SLAM_TIME);
        fRatio = std::clamp(fRatio, 0.f, 1.f);
        const float fEaseIn = fRatio * fRatio * fRatio;
        m_tInfo.vPoint = m_vMoveStart + (m_vSlamTarget - m_vMoveStart) * fEaseIn;

        if (m_dStateElapseTime >= SLAM_TIME + IMPACT_HOLD_TIME)
            m_eNextState = ERMA_HAND_STATE::RETURN;
        break;
    }

    case ERMA_HAND_STATE::RETURN:
    {
        float fRatio = static_cast<float>(m_dStateElapseTime / RETURN_TIME);
        fRatio = std::clamp(fRatio, 0.f, 1.f);
        const float fEaseOut = 1.f - (1.f - fRatio) * (1.f - fRatio);
        m_tInfo.vPoint = m_vMoveStart + (m_vAnchor - m_vMoveStart) * fEaseOut;

        if (fRatio >= 1.f)
            m_eNextState = ERMA_HAND_STATE::WAIT;
        break;
    }

    default:
        break;
    }
}

void CErmaHand::RenderAttackEffect(Graphics* pGraphics, const VEC& vScroll)
{
    if (m_eCurState != ERMA_HAND_STATE::READY &&
        m_eCurState != ERMA_HAND_STATE::SLAM &&
        m_eCurState != ERMA_HAND_STATE::RETURN)
    {
        return;
    }

    const TCHAR* pShadowKey = (m_eSide == ERMA_HAND_SIDE::LEFT)
        ? L"Erma_Hand_Shadow_L"
        : L"Erma_Hand_Shadow_R";
    Image* pShadow = CImgMgr::GetInstance()->FindImg(pShadowKey);

    if (pShadow != nullptr)
    {
        constexpr float SHADOW_X = 42.f;
        constexpr float SHADOW_Y = 17.f;
        const VEC vShadowSize = VEC{ SHADOW_X, SHADOW_Y } * PIXEL_SCALE;
        const RectF rcShadow{
            m_vSlamTarget.fX - vShadowSize.fX * 0.5f + vScroll.fX,
            m_vSlamTarget.fY - vShadowSize.fY * 0.5f + vScroll.fY,
            vShadowSize.fX,
            vShadowSize.fY
        };

        pGraphics->DrawImage(
            pShadow, rcShadow,
            0.f, 0.f,
            SHADOW_X, SHADOW_Y,
            UnitPixel);
    }

    if (m_eCurState != ERMA_HAND_STATE::READY ||
        m_dStateElapseTime < ALERT_START_TIME)
    {
        return;
    }

    Image* pAlert = CImgMgr::GetInstance()->FindImg(L"Erma_Alert_Square");
    if (pAlert == nullptr)
        return;

    constexpr float CELL_X = 32.f;
    constexpr float CELL_Y = 32.f;
    float fRatio = static_cast<float>(
        (m_dStateElapseTime - ALERT_START_TIME) /
        (READY_TIME - ALERT_START_TIME));
    fRatio = std::clamp(fRatio, 0.f, 1.f);
    const int iFrame = std::clamp(static_cast<int>(fRatio * 4.f), 0, 3);
    const VEC vAlertSize = VEC{ CELL_X, CELL_Y } * PIXEL_SCALE;
    const RectF rcAlert{
        m_vSlamTarget.fX - vAlertSize.fX * 0.5f + vScroll.fX,
        m_vSlamTarget.fY - vAlertSize.fY * 0.5f + vScroll.fY,
        vAlertSize.fX,
        vAlertSize.fY
    };

    ColorMatrix alertColor = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.55f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.55f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.55f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    ImageAttributes alertAttribute;
    alertAttribute.SetColorMatrix(
        &alertColor,
        ColorMatrixFlagsDefault,
        ColorAdjustTypeBitmap);

    pGraphics->DrawImage(
        pAlert, rcAlert,
        CELL_X * iFrame, 0.f,
        CELL_X, CELL_Y,
        UnitPixel,
        &alertAttribute);
}
