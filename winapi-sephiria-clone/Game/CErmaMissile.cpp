#include "pch.h"
#include "CErmaMissile.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"

namespace
{
    constexpr double WARNING_TIME = 1.2;
    constexpr int WARNING_STEP_COUNT = 4;
    constexpr int WARNING_FRAME_STEP = 5;

    constexpr double STRIKE_FRAME_TIME = 0.06;
    constexpr int STRIKE_FRAME_COUNT = 17;
    constexpr double STRIKE_TIME = STRIKE_FRAME_TIME * STRIKE_FRAME_COUNT;
    constexpr int IMPACT_FRAME = 3;

    constexpr float WARNING_CELL_X = 66.f;
    constexpr float WARNING_CELL_Y = 34.f;
    constexpr float STRIKE_CELL_X = 51.f;
    constexpr float STRIKE_CELL_Y = 103.f;

    constexpr float IMPACT_WIDTH = 220.f;
    constexpr float IMPACT_HEIGHT = 120.f;

    constexpr float ARENA_LEFT = 3350.f;
    constexpr float ARENA_RIGHT = 4570.f;
    constexpr float ARENA_TOP = 1080.f;
    constexpr float ARENA_BOTTOM = 2300.f;
    constexpr float TARGET_MARGIN_X = 170.f;
    constexpr float TARGET_MARGIN_Y = 80.f;
}

CErmaMissile::CErmaMissile()
    : m_eState(ERMA_MISSILE_STATE::DELAY),
    m_vImpactPoint{},
    m_dStateElapseTime(0.),
    m_dStartDelay(0.),
    m_bDamageApplied(false)
{
}

CErmaMissile::~CErmaMissile()
{
    Release();
}

void CErmaMissile::Initialize()
{
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };
    m_tStat = { 1, 1, 20 };

    m_eRender = RENDERID::EFFECT;
    m_iRenderLayer = 3;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Effect/Boss/Alert_Missile.png",
        L"Erma_Missile_Alert");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Effect/Boss/Missile.png",
        L"Erma_Missile_Strike");
}

int CErmaMissile::Update()
{
    if (m_bDead)
        return DEAD;

    m_dStateElapseTime += DT;

    switch (m_eState)
    {
    case ERMA_MISSILE_STATE::DELAY:
        if (m_dStateElapseTime >= m_dStartDelay)
            ChangeState(ERMA_MISSILE_STATE::WARNING);
        break;

    case ERMA_MISSILE_STATE::WARNING:
        if (m_dStateElapseTime >= WARNING_TIME)
            ChangeState(ERMA_MISSILE_STATE::STRIKE);
        break;

    case ERMA_MISSILE_STATE::STRIKE:
        TryDamageTarget();
        if (m_dStateElapseTime >= STRIKE_TIME)
            m_bDead = true;
        break;
    }

    __super::UpdateRect();
    return m_bDead ? DEAD : NOEVENT;
}

void CErmaMissile::LateUpdate()
{
}

void CErmaMissile::Render(Graphics* pGraphics)
{
    if (pGraphics == nullptr || m_eState == ERMA_MISSILE_STATE::DELAY)
        return;

    const VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    switch (m_eState)
    {
    case ERMA_MISSILE_STATE::WARNING:
        RenderWarning(pGraphics, vScroll);
        break;
    case ERMA_MISSILE_STATE::STRIKE:
        RenderStrike(pGraphics, vScroll);
        break;
    default:
        break;
    }

#ifdef _DEBUG
    if (m_eState == ERMA_MISSILE_STATE::STRIKE && !m_bDamageApplied)
    {
        Pen pen(Color(180, 255, 80, 40), 2.f);
        pGraphics->DrawRectangle(
            &pen,
            static_cast<int>(m_tRect.left + vScroll.fX),
            static_cast<int>(m_tRect.top + vScroll.fY),
            static_cast<int>(m_tInfo.vSize.fX),
            static_cast<int>(m_tInfo.vSize.fY));
    }
#endif
}

void CErmaMissile::Release()
{
}

void CErmaMissile::Configure(
    const VEC& vImpactPoint,
    double dStartDelay)
{
    m_vImpactPoint = {
        std::clamp(
            vImpactPoint.fX,
            ARENA_LEFT + TARGET_MARGIN_X,
            ARENA_RIGHT - TARGET_MARGIN_X),
        std::clamp(
            vImpactPoint.fY,
            ARENA_TOP + TARGET_MARGIN_Y,
            ARENA_BOTTOM - TARGET_MARGIN_Y)
    };
    m_tInfo.vPoint = m_vImpactPoint;
    m_dStartDelay = (std::max)(0., dStartDelay);
}

void CErmaMissile::ChangeState(ERMA_MISSILE_STATE eNextState)
{
    m_eState = eNextState;
    m_dStateElapseTime = 0.;

    if (m_eState == ERMA_MISSILE_STATE::STRIKE)
    {
        m_tInfo.vPoint = m_vImpactPoint;
        m_tInfo.vSize = { IMPACT_WIDTH, IMPACT_HEIGHT };
    }
    else
    {
        m_tInfo.vSize = { 0.f, 0.f };
    }
}

void CErmaMissile::TryDamageTarget()
{
    if (m_bDamageApplied)
        return;

    const int iFrame = (std::min)(
        static_cast<int>(m_dStateElapseTime / STRIKE_FRAME_TIME),
        STRIKE_FRAME_COUNT - 1);
    if (iFrame < IMPACT_FRAME)
        return;

    m_bDamageApplied = true;

    CObj* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (pPlayer == nullptr)
        return;

    RECT rcCollision{};
    if (IntersectRect(&rcCollision, &m_tRect, &pPlayer->GetRect()))
        pPlayer->SetDamage(m_tStat.iAtk, this);
}

void CErmaMissile::RenderWarning(Graphics* pGraphics, const VEC& vScroll) const
{
    Image* pAlert = CImgMgr::GetInstance()->FindImg(L"Erma_Missile_Alert");
    if (pAlert == nullptr)
        return;

    const float fRatio = std::clamp(
        static_cast<float>(m_dStateElapseTime / WARNING_TIME),
        0.f,
        1.f);
    const int iStep = std::clamp(
        static_cast<int>(fRatio * WARNING_STEP_COUNT),
        0,
        WARNING_STEP_COUNT - 1);
    const int iFrame = iStep * WARNING_FRAME_STEP;
    const VEC vDrawSize = VEC{ WARNING_CELL_X, WARNING_CELL_Y } * PIXEL_SCALE;
    const RectF rcDest{
        m_vImpactPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
        m_vImpactPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
        vDrawSize.fX,
        vDrawSize.fY
    };

    ColorMatrix alertColor = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.55f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.55f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.8f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    ImageAttributes alertAttribute;
    alertAttribute.SetColorMatrix(
        &alertColor,
        ColorMatrixFlagsDefault,
        ColorAdjustTypeBitmap);

    pGraphics->DrawImage(
        pAlert, rcDest,
        WARNING_CELL_X * iFrame, 0.f,
        WARNING_CELL_X, WARNING_CELL_Y,
        UnitPixel,
        &alertAttribute);
}

void CErmaMissile::RenderStrike(Graphics* pGraphics, const VEC& vScroll) const
{
    Image* pMissile = CImgMgr::GetInstance()->FindImg(L"Erma_Missile_Strike");
    if (pMissile == nullptr)
        return;

    const int iFrame = (std::min)(
        static_cast<int>(m_dStateElapseTime / STRIKE_FRAME_TIME),
        STRIKE_FRAME_COUNT - 1);
    const VEC vDrawSize = VEC{ STRIKE_CELL_X, STRIKE_CELL_Y } * PIXEL_SCALE;
    const RectF rcDest{
        m_vImpactPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
        m_vImpactPoint.fY - vDrawSize.fY + vScroll.fY,
        vDrawSize.fX,
        vDrawSize.fY
    };

    pGraphics->DrawImage(
        pMissile, rcDest,
        STRIKE_CELL_X * iFrame, 0.f,
        STRIKE_CELL_X, STRIKE_CELL_Y,
        UnitPixel);
}
