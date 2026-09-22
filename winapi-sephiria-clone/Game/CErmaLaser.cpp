#include "pch.h"
#include "CErmaLaser.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"

namespace
{
    constexpr float ARENA_TOP = 1080.f;
    constexpr float ARENA_BOTTOM = 2300.f;

    constexpr double WARNING_TIME = 0.85;
    constexpr double BEAM_GROW_TIME = 0.14;
    constexpr double ACTIVE_TIME = 3.0;
    constexpr double FADE_TIME = 0.25;
    constexpr double FRAME_TIME = 0.08;
    constexpr int FRAME_COUNT = 12;

    constexpr float BODY_CELL_X = 48.f;
    constexpr float BODY_CELL_Y = 32.f;
    constexpr float HEAD_CELL_X = 48.f;
    constexpr float HEAD_CELL_Y = 48.f;
    constexpr float LASER_SPRITE_SCALE = 4.f;

    constexpr float HIT_HEIGHT = 100.f;
    constexpr float LASER_MARGIN_Y = 120.f;
}

CErmaLaser::CErmaLaser()
    : m_eState(ERMA_LASER_STATE::DELAY),
    m_fLaserStartX(0.f),
    m_fLaserEndX(0.f),
    m_fLaserY(0.f),
    m_fLaserStartY(0.f),
    m_fLaserEndY(0.f),
    m_dStateElapseTime(0.),
    m_dStartDelay(0.),
    m_bFromLeft(true),
    m_bDamageApplied(false)
{
}

CErmaLaser::~CErmaLaser()
{
    Release();
}

void CErmaLaser::Initialize()
{
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };
    m_tStat = { 1, 1, 20 };

    m_eRender = RENDERID::EFFECT;
    m_iRenderLayer = 4;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Effect/Laser/BigGolem_Laser_Body.png",
        L"Erma_Laser_Body");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Effect/Laser/BigGolem_Laser_Head.png",
        L"Erma_Laser_Head");
}

int CErmaLaser::Update()
{
    if (m_bDead)
        return DEAD;

    m_dStateElapseTime += DT;

    switch (m_eState)
    {
    case ERMA_LASER_STATE::DELAY:
        if (m_dStateElapseTime >= m_dStartDelay)
            ChangeState(ERMA_LASER_STATE::WARNING);
        break;

    case ERMA_LASER_STATE::WARNING:
        if (m_dStateElapseTime >= WARNING_TIME)
            ChangeState(ERMA_LASER_STATE::ACTIVE);
        break;

    case ERMA_LASER_STATE::ACTIVE:
        UpdateLaserPosition();
        UpdateHitBox();
        if (m_dStateElapseTime >= ACTIVE_TIME)
            m_bDead = true;
        break;
    }

    __super::UpdateRect();
    return m_bDead ? DEAD : NOEVENT;
}

void CErmaLaser::LateUpdate()
{
    if (m_eState != ERMA_LASER_STATE::ACTIVE || m_bDamageApplied)
        return;

    CObj* pPlayer = CObjMgr::GetInstance()->GetPlayer();
    if (pPlayer == nullptr)
        return;

    RECT rcCollision{};
    if (IntersectRect(&rcCollision, &m_tRect, &pPlayer->GetRect()))
    {
        pPlayer->SetDamage(m_tStat.iAtk, this);
        m_bDamageApplied = true;
    }
}

void CErmaLaser::Render(Graphics* pGraphics)
{
    if (pGraphics == nullptr || m_eState == ERMA_LASER_STATE::DELAY)
        return;

    const VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    if (m_eState == ERMA_LASER_STATE::WARNING)
        RenderWarning(pGraphics, vScroll);
    else if (m_eState == ERMA_LASER_STATE::ACTIVE)
        RenderLaser(pGraphics, vScroll);

#ifdef _DEBUG
    if (m_eState == ERMA_LASER_STATE::ACTIVE)
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

void CErmaLaser::Release()
{
}

void CErmaLaser::Configure(
    float fStartX,
    float fEndX,
    float fStartY,
    float fEndY,
    double dStartDelay,
    bool bFromLeft)
{
    m_fLaserStartX = fStartX;
    m_fLaserEndX = fEndX;
    m_fLaserStartY = std::clamp(
        fStartY,
        ARENA_TOP + LASER_MARGIN_Y,
        ARENA_BOTTOM - LASER_MARGIN_Y);
    m_fLaserEndY = std::clamp(
        fEndY,
        ARENA_TOP + LASER_MARGIN_Y,
        ARENA_BOTTOM - LASER_MARGIN_Y);
    m_fLaserY = m_fLaserStartY;
    m_dStartDelay = (std::max)(0., dStartDelay);
    m_bFromLeft = bFromLeft;
    m_tInfo.vPoint = { m_fLaserStartX, m_fLaserY };
}

void CErmaLaser::ChangeState(ERMA_LASER_STATE eNextState)
{
    m_eState = eNextState;
    m_dStateElapseTime = 0.;

    if (m_eState == ERMA_LASER_STATE::ACTIVE)
    {
        m_fLaserY = m_fLaserStartY;
        UpdateHitBox();
    }
    else
    {
        m_tInfo.vSize = { 0.f, 0.f };
    }
}

void CErmaLaser::UpdateLaserPosition()
{
    const double dSweepTime = ACTIVE_TIME - FADE_TIME;
    float fRatio = static_cast<float>(m_dStateElapseTime / dSweepTime);
    fRatio = std::clamp(fRatio, 0.f, 1.f);

    // Smooth acceleration and deceleration keep the crossing readable while
    // both hands sweep through the center of the arena.
    const float fSmoothRatio = fRatio * fRatio * (3.f - 2.f * fRatio);
    m_fLaserY = m_fLaserStartY +
        (m_fLaserEndY - m_fLaserStartY) * fSmoothRatio;
}

void CErmaLaser::UpdateHitBox()
{
    const float fBeamRatio = GetBeamRatio();
    const float fBeamDeltaX =
        (m_fLaserEndX - m_fLaserStartX) * fBeamRatio;
    const float fBeamWidth = std::abs(fBeamDeltaX);
    const float fCenterX = m_fLaserStartX + fBeamDeltaX * 0.5f;

    m_tInfo.vPoint = { fCenterX, m_fLaserY };
    m_tInfo.vSize = { fBeamWidth, HIT_HEIGHT };
}

float CErmaLaser::GetBeamRatio() const
{
    if (m_eState != ERMA_LASER_STATE::ACTIVE)
        return 0.f;

    const float fRatio = static_cast<float>(m_dStateElapseTime / BEAM_GROW_TIME);
    return std::clamp(fRatio, 0.f, 1.f);
}

float CErmaLaser::GetBeamAlpha() const
{
    if (m_eState != ERMA_LASER_STATE::ACTIVE)
        return 0.f;

    const double dFadeStart = ACTIVE_TIME - FADE_TIME;
    if (m_dStateElapseTime <= dFadeStart)
        return 1.f;

    const float fAlpha = static_cast<float>(
        (ACTIVE_TIME - m_dStateElapseTime) / FADE_TIME);
    return std::clamp(fAlpha, 0.f, 1.f);
}

void CErmaLaser::RenderWarning(Graphics* pGraphics, const VEC& vScroll) const
{
    float fRatio = static_cast<float>(m_dStateElapseTime / WARNING_TIME);
    fRatio = std::clamp(fRatio, 0.f, 1.f);
    const float fPulse = 0.5f + 0.5f * std::sin(
        static_cast<float>(m_dStateElapseTime * 22.));
    const BYTE byAlpha = static_cast<BYTE>(135.f + 80.f * fRatio * fPulse);
    const float fWidth = 3.f + 3.f * fRatio;
    const float fStartX = m_fLaserStartX + vScroll.fX;
    const float fEndX = m_fLaserEndX + vScroll.fX;
    const float fY = m_fLaserY + vScroll.fY;

    Pen warningPen(Color(byAlpha, 255, 70, 70), fWidth);
    pGraphics->DrawLine(&warningPen, fStartX, fY, fEndX, fY);
}

void CErmaLaser::RenderLaser(Graphics* pGraphics, const VEC& vScroll) const
{
    Image* pBody = CImgMgr::GetInstance()->FindImg(L"Erma_Laser_Body");
    Image* pHead = CImgMgr::GetInstance()->FindImg(L"Erma_Laser_Head");
    if (pBody == nullptr || pHead == nullptr)
        return;

    const int iFrame = static_cast<int>(m_dStateElapseTime / FRAME_TIME) % FRAME_COUNT;
    const float fAlpha = GetBeamAlpha();

    ColorMatrix colorMatrix = {
        1.f, 0.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 0.f, fAlpha, 0.f,
        0.f, 0.f, 0.f, 0.f, 1.f
    };
    ImageAttributes imageAttribute;
    imageAttribute.SetColorMatrix(
        &colorMatrix,
        ColorMatrixFlagsDefault,
        ColorAdjustTypeBitmap);

    // The source frames are vertical. Rotate every frame by 90 degrees and
    // tile its original height along the horizontal beam axis.
    const float fBodyWidth = BODY_CELL_X * LASER_SPRITE_SCALE;
    const float fBodyHeight = BODY_CELL_Y * LASER_SPRITE_SCALE;
    const float fBodySegmentLength = fBodyHeight;
    const float fBeamRatio = GetBeamRatio();
    const float fWorldHeadX = m_fLaserStartX +
        (m_fLaserEndX - m_fLaserStartX) * fBeamRatio;
    const float fWorldBeamLeft = (std::min)(m_fLaserStartX, fWorldHeadX);
    const float fWorldBeamRight = (std::max)(m_fLaserStartX, fWorldHeadX);
    const float fScreenLeft = fWorldBeamLeft + vScroll.fX;
    const float fScreenRight = fWorldBeamRight + vScroll.fX;
    const float fScreenY = m_fLaserY + vScroll.fY;

    const GraphicsState beamGraphicsState = pGraphics->Save();
    RectF rcBeamClip{
        fScreenLeft,
        fScreenY - fBodyWidth * 0.5f,
        (std::max)(0.f, fScreenRight - fScreenLeft),
        fBodyWidth
    };
    pGraphics->SetClip(rcBeamClip, CombineModeIntersect);

    for (float fX = fScreenLeft; fX < fScreenRight; fX += fBodySegmentLength)
    {
        const GraphicsState tileGraphicsState = pGraphics->Save();
        pGraphics->TranslateTransform(fX + fBodySegmentLength * 0.5f, fScreenY);
        pGraphics->RotateTransform(m_bFromLeft ? 90.f : -90.f);
        const RectF rcBody{
            -fBodyWidth * 0.5f,
            -fBodyHeight * 0.5f,
            fBodyWidth,
            fBodyHeight
        };

        pGraphics->DrawImage(
            pBody, rcBody,
            BODY_CELL_X * iFrame, 0.f,
            BODY_CELL_X, BODY_CELL_Y,
            UnitPixel,
            &imageAttribute);
        pGraphics->Restore(tileGraphicsState);
    }
    pGraphics->Restore(beamGraphicsState);

    const float fHeadSize = HEAD_CELL_X * LASER_SPRITE_SCALE;
    // Keep the head at the advancing tip of the beam. The emission side is
    // filled by body tiles, while the opposite edge grows toward the target.
    const float fHeadX = fWorldHeadX + vScroll.fX;
    const GraphicsState graphicsState = pGraphics->Save();
    pGraphics->TranslateTransform(fHeadX, fScreenY);
    pGraphics->RotateTransform(m_bFromLeft ? 90.f : -90.f);
    const RectF rcHead{
        -fHeadSize * 0.5f,
        -fHeadSize * 0.5f,
        fHeadSize,
        fHeadSize
    };
    pGraphics->DrawImage(
        pHead, rcHead,
        HEAD_CELL_X * iFrame, 0.f,
        HEAD_CELL_X, HEAD_CELL_Y,
        UnitPixel,
        &imageAttribute);
    pGraphics->Restore(graphicsState);
}
