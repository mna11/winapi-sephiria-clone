#include "pch.h"
#include "CErmaLaser.h"

#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"

namespace
{
    constexpr float ARENA_LEFT = 3350.f;
    constexpr float ARENA_RIGHT = 4570.f;
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
    constexpr float LASER_SPRITE_SCALE = 3.f;

    constexpr float HAND_CELL_X = 62.f;
    constexpr float HAND_CELL_Y = 64.f;
    constexpr int HAND_READY_LAST_FRAME = 5;
    constexpr int HAND_FIRE_FIRST_FRAME = 5;
    constexpr int HAND_FIRE_FRAME_COUNT = 7;
    constexpr int HAND_EXIT_FIRST_FRAME = 12;
    constexpr int HAND_EXIT_FRAME_COUNT = 6;
    constexpr float HAND_MOVE_DISTANCE = 260.f;
    constexpr float HAND_TIP_OFFSET_X = 70.f;

    constexpr float HIT_HEIGHT = 70.f;
    constexpr float LASER_MARGIN_Y = 120.f;
}

CErmaLaser::CErmaLaser()
    : m_eState(ERMA_LASER_STATE::DELAY),
    m_fLaserY(0.f),
    m_fLaserStartY(0.f),
    m_fLaserEndY(0.f),
    m_dStateElapseTime(0.),
    m_dStartDelay(0.),
    m_fHandStartY(0.f),
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
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Hand/BigGolem_Hand_LEFT.png",
        L"Erma_Golem_Hand_L");
    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Hand/BigGolem_Hand_RIGHT.png",
        L"Erma_Golem_Hand_R");
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
    RenderHand(pGraphics, vScroll);

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
    float fStartY,
    float fEndY,
    double dStartDelay,
    bool bFromLeft)
{
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
    m_fHandStartY = m_fLaserY + (m_bFromLeft
        ? -HAND_MOVE_DISTANCE
        : HAND_MOVE_DISTANCE);
    m_tInfo.vPoint = { (ARENA_LEFT + ARENA_RIGHT) * 0.5f, m_fLaserY };
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
    const float fBeamWidth = (ARENA_RIGHT - ARENA_LEFT) * fBeamRatio;
    const float fCenterX = m_bFromLeft
        ? ARENA_LEFT + fBeamWidth * 0.5f
        : ARENA_RIGHT - fBeamWidth * 0.5f;

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

float CErmaLaser::GetHandY() const
{
    if (m_eState != ERMA_LASER_STATE::WARNING)
        return m_fLaserY;

    float fRatio = static_cast<float>(m_dStateElapseTime / WARNING_TIME);
    fRatio = std::clamp(fRatio, 0.f, 1.f);
    const float fEaseOut = 1.f - (1.f - fRatio) * (1.f - fRatio) * (1.f - fRatio);
    return m_fHandStartY + (m_fLaserY - m_fHandStartY) * fEaseOut;
}

void CErmaLaser::RenderHand(Graphics* pGraphics, const VEC& vScroll) const
{
    const TCHAR* pHandKey = m_bFromLeft
        ? L"Erma_Golem_Hand_L"
        : L"Erma_Golem_Hand_R";
    Image* pHand = CImgMgr::GetInstance()->FindImg(pHandKey);
    if (pHand == nullptr)
        return;

    int iHandFrame = 0;
    float fAlpha = 1.f;

    if (m_eState == ERMA_LASER_STATE::WARNING)
    {
        float fRatio = static_cast<float>(m_dStateElapseTime / WARNING_TIME);
        fRatio = std::clamp(fRatio, 0.f, 1.f);
        iHandFrame = std::clamp(
            static_cast<int>(fRatio * (HAND_READY_LAST_FRAME + 1)),
            0,
            HAND_READY_LAST_FRAME);
    }
    else
    {
        const double dFadeStart = ACTIVE_TIME - FADE_TIME;
        if (m_dStateElapseTime >= dFadeStart)
        {
            const float fExitRatio = std::clamp(
                static_cast<float>((m_dStateElapseTime - dFadeStart) / FADE_TIME),
                0.f,
                1.f);
            iHandFrame = HAND_EXIT_FIRST_FRAME + (std::min)(
                static_cast<int>(fExitRatio * HAND_EXIT_FRAME_COUNT),
                HAND_EXIT_FRAME_COUNT - 1);
            fAlpha = GetBeamAlpha();
        }
        else
        {
            iHandFrame = HAND_FIRE_FIRST_FRAME +
                static_cast<int>(m_dStateElapseTime / FRAME_TIME) %
                HAND_FIRE_FRAME_COUNT;
        }
    }

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

    const float fHandWidth = HAND_CELL_X * PIXEL_SCALE;
    const float fHandHeight = HAND_CELL_Y * PIXEL_SCALE;
    const float fHandX = (m_bFromLeft
        ? ARENA_LEFT - HAND_TIP_OFFSET_X
        : ARENA_RIGHT + HAND_TIP_OFFSET_X) + vScroll.fX;
    const float fHandY = GetHandY() + vScroll.fY;
    const RectF rcHand{
        fHandX - fHandWidth * 0.5f,
        fHandY - fHandHeight * 0.5f,
        fHandWidth,
        fHandHeight
    };

    pGraphics->DrawImage(
        pHand, rcHand,
        HAND_CELL_X * iHandFrame, 0.f,
        HAND_CELL_X, HAND_CELL_Y,
        UnitPixel,
        &imageAttribute);
}

void CErmaLaser::RenderWarning(Graphics* pGraphics, const VEC& vScroll) const
{
    float fRatio = static_cast<float>(m_dStateElapseTime / WARNING_TIME);
    fRatio = std::clamp(fRatio, 0.f, 1.f);
    const float fPulse = 0.5f + 0.5f * std::sin(
        static_cast<float>(m_dStateElapseTime * 22.));
    const BYTE byAlpha = static_cast<BYTE>(135.f + 80.f * fRatio * fPulse);
    const float fWidth = 3.f + 3.f * fRatio;
    const float fStartX = ARENA_LEFT + vScroll.fX;
    const float fEndX = ARENA_RIGHT + vScroll.fX;
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
    const float fArenaWidth = ARENA_RIGHT - ARENA_LEFT;
    const float fWorldBeamLeft = m_bFromLeft
        ? ARENA_LEFT
        : ARENA_RIGHT - fArenaWidth * fBeamRatio;
    const float fWorldBeamRight = m_bFromLeft
        ? ARENA_LEFT + fArenaWidth * fBeamRatio
        : ARENA_RIGHT;
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
    const float fHeadX = (m_bFromLeft ? ARENA_LEFT : ARENA_RIGHT) + vScroll.fX;
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
