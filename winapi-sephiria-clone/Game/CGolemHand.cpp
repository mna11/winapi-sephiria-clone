#include "pch.h"
#include "CGolemHand.h"

#include "CGolem.h"
#include "CEffect.h"

#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CGolemHand::CGolemHand()
    : CState(GOLEM_HAND_STATE::END, GOLEM_HAND_STATE::IDLE),
    m_pOwner(nullptr), m_bLeft(false), m_pLaserFrameKey(nullptr),
    m_dStateTime(0.), m_dSlamReadyTime(0.), m_dSlamTime(0.), 
    m_dLaserReadyTime(0.), m_dLaserTime(0.), m_dLaserCreateTime(0.), m_dLaserEndTime(0.)
{
    ZeroMemory(&m_vIdlePoint, sizeof(VEC));
    ZeroMemory(&m_vSlamPoint, sizeof(VEC));
    ZeroMemory(&m_vSlamStartPoint, sizeof(VEC));
    ZeroMemory(&m_vSlamReadyStartPoint, sizeof(VEC));
    ZeroMemory(&m_tLaserFrame, sizeof(FRAME));

    m_vecAtkRect.resize(toUType(GOLEM_HAND_ATK_RECT::END));
}

CGolemHand::~CGolemHand()
{
}

void CGolemHand::Initialize()
{
    m_tInfo = { 0.f, 0.f, 150.f, 150.f };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 1;

    // 주먹 찍기 시간
    m_dSlamReadyTime = 0.75f;
    m_dSlamTime = 0.4f;

    // 레이저 시간
    m_dLaserReadyTime = 0.85;   // 지정 위치로 이동하는 시간
    m_dLaserCreateTime = 0.5;   // 레이저 만드는 시간
    m_dLaserTime = 5.0;         // 레이저 움직이는 시간 + vecAtk 활성화 시간
    m_dLaserEndTime = 0.5;      // 레이저 없어지는 시간

    m_tStat = { 0, 0, 20 };     // 주먹 데미지

    m_bWallCollisionEnable = false;
}

int CGolemHand::Update()
{
    m_vPrePoint = m_tInfo.vPoint;

    ApplyChange();
    UpdateTime();
    Move();
    Attack();

    UpdateFrame();
    __super::UpdateRect();
	return NOEVENT;
}

void CGolemHand::LateUpdate()
{
}

void CGolemHand::Render(Graphics* pGraphics)
{
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
    // 피격 충돌 박스 렌더링
    SolidBrush WhiteBrush(Color(255, 255, 255, 255));
    pGraphics->FillRectangle(&WhiteBrush, (int)(m_tRect.left + vScroll.fX),
        (int)(m_tRect.top + vScroll.fY),
        (int)m_tInfo.vSize.fX,
        (int)m_tInfo.vSize.fY);

    // 공격 충돌 박스 렌더링
    SolidBrush BlackBrush(Color(255, 0, 0, 0));
    for (int i = 0; i < toUType(GOLEM_HAND_ATK_RECT::END); ++i)
    {
        pGraphics->FillRectangle(&BlackBrush, (int)(m_vecAtkRect[i].left + vScroll.fX),
            (int)(m_vecAtkRect[i].top + vScroll.fY),
            (int)(m_vecAtkRect[i].right - m_vecAtkRect[i].left),
            (int)(m_vecAtkRect[i].bottom - m_vecAtkRect[i].top));
    }
#endif // _DEBUG


    // 손 렌더링
    Image* pHeadImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
    if (nullptr == pHeadImg)
        return;

    VEC vCellSize{ 62.f, 64.f };
    VEC vImgSize = vCellSize * PIXEL_SCALE * 0.8;
    VEC vOffset{ vScroll.fX, vScroll.fY };
    RectF DestRect = { m_tInfo.vPoint.fX + vOffset.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY + vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };

    if (m_bHit && m_dIframeTime > 0.)
    {
        float fRatio = clamp(static_cast<float>(m_dHitElapseTime / m_dIframeTime), 0.f, 1.f);
        float fAmplitude = vImgSize.fY * 0.1f;
        float fDecay = (1.f - fRatio) * (1.f - fRatio);
        float fOffset = -fAmplitude * sinf(fRatio * PI) * fDecay;

        DestRect.X -= fOffset * 0.5f;
        DestRect.Y += fOffset;
        DestRect.Width += fOffset;
        DestRect.Height -= fOffset;
    }

    ImageAttributes* pImgAttr = m_bHit ? &m_imgAttrHit : nullptr;

    pGraphics->DrawImage(
        pHeadImg, DestRect,
        vCellSize.fX * m_tFrame.iStart,
        vCellSize.fY * m_tFrame.iMotion,
        vCellSize.fX, vCellSize.fY,
        UnitPixel, pImgAttr
    );

    // 레이저 렌더링
    RenderLaser(pGraphics);
}

void CGolemHand::RenderLaser(Graphics* pGraphics)
{
    // 레이저 관련 상태가 아니면 리턴
    if (m_eCurState != GOLEM_HAND_STATE::LASER_CREATE && m_eCurState != GOLEM_HAND_STATE::LASER && m_eCurState != GOLEM_HAND_STATE::LASER_END)
        return;

    Image* pImg = CImgMgr::GetInstance()->FindImg(m_pLaserFrameKey);
    if (pImg == nullptr)
        return;

    const VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
    VEC vCellSize{ 416.f, 48.f };
    VEC vImgSize = vCellSize * PIXEL_SCALE;

    VEC vOffset{ m_bLeft ? 0.f : -vImgSize.fX, 20.f + -vImgSize.fY * 0.5f };
    RectF DestRect{ m_tInfo.vPoint.fX + vOffset.fX + vScroll.fX, m_tInfo.vPoint.fY + vOffset.fY + vScroll.fY, vImgSize.fX, vImgSize.fY };
    
    pGraphics->DrawImage(
        pImg,
        DestRect,
        vCellSize.fX * m_tLaserFrame.iStart,
        0.f,
        vCellSize.fX,
        vCellSize.fY,
        UnitPixel
    );
}

void CGolemHand::Release()
{
}

void CGolemHand::UpdateTime()
{
    if (nullptr == m_pTarget)
        return;

    if (m_bHit)
    {
        m_dHitElapseTime += DT;

        if (m_dHitElapseTime >= m_dIframeTime)
        {
            m_bHit = false;
            m_dHitElapseTime = 0.;
        }
    }

    m_dStateTime += DT;

    switch (m_eCurState)
    {
    case GOLEM_HAND_STATE::IDLE:
    {
        break;
    }
    case GOLEM_HAND_STATE::SLAM_READY:
    {
        if (m_dStateTime >= m_dSlamReadyTime)
            RequestChange(GOLEM_HAND_STATE::SLAM);
        break;
    }
    case GOLEM_HAND_STATE::SLAM:
    {
        if (m_dStateTime >= m_dSlamTime)
            RequestChange(GOLEM_HAND_STATE::RETURN);

        break;
    }
    case GOLEM_HAND_STATE::LASER_READY:
        if (m_dStateTime >= m_dLaserReadyTime)
            RequestChange(GOLEM_HAND_STATE::LASER_CREATE);
        break;

    case GOLEM_HAND_STATE::LASER_CREATE:
        if (m_dStateTime >= m_dLaserCreateTime)
            RequestChange(GOLEM_HAND_STATE::LASER);
        break;

    case GOLEM_HAND_STATE::LASER:
        if (m_dStateTime >= m_dLaserTime)
            RequestChange(GOLEM_HAND_STATE::LASER_END);
        break;

    case GOLEM_HAND_STATE::LASER_END:
        if (m_dStateTime >= m_dLaserEndTime)
            RequestChange(GOLEM_HAND_STATE::RETURN);
        break;
    case GOLEM_HAND_STATE::RETURN:
        break;
    default:
        break;
    }
}

void CGolemHand::Move()
{
    if (nullptr == m_pTarget)
        return;

    switch (m_eCurState)
    {
    case GOLEM_HAND_STATE::IDLE:
    {
        break;
    }
    case GOLEM_HAND_STATE::SLAM_READY:
    {
        m_vSlamPoint = m_pTarget->GetInfo().vPoint;
        VEC vFollowPoint = m_vSlamPoint + VEC{ 0.f, -280.f };
        float fRatio = clamp(static_cast<float>(m_dStateTime / m_dSlamReadyTime), 0.f, 1.f);
        m_tInfo.vPoint = m_vSlamReadyStartPoint * (1 - fRatio) + vFollowPoint * fRatio;

        break;
    }
    case GOLEM_HAND_STATE::SLAM:
    {
        float fRatio = clamp(static_cast<float>(m_dStateTime / m_dSlamTime), 0.f, 1.f);
        float fWeight = fRatio * fRatio * fRatio;

        m_tInfo.vPoint = m_vSlamStartPoint * (1.f - fWeight) + m_vSlamPoint * fWeight;

        break;
    }
    case GOLEM_HAND_STATE::LASER_READY:
    {
        float fRatio = clamp(static_cast<float>(m_dStateTime / m_dLaserReadyTime), 0.f, 1.f);

        m_tInfo.vPoint = m_vLaserReadyStart * (1.f - fRatio) + m_vLaserStart * fRatio;
        break;
    }
    case GOLEM_HAND_STATE::LASER:
    {
        double dWayTime = 2.5;
        // 0에서 1은 Start->End, 1에서 2는 End->Start
        float fCycle = fmod(m_dStateTime / dWayTime, 2.);
        float fRatio(0.f);

        if (fCycle <= 1.f)
            fRatio = fCycle;
        else
            fRatio = 2.f - fCycle;

        m_tInfo.vPoint = m_vLaserStart * (1.f - fRatio) + m_vLaserEnd * fRatio;

        break;
    }
    case GOLEM_HAND_STATE::LASER_CREATE:
    case GOLEM_HAND_STATE::LASER_END:
        break;
    case GOLEM_HAND_STATE::RETURN: // IdlePoint로 돌아오기
    {
        VEC vDistance = m_vIdlePoint - m_tInfo.vPoint;
        float fDistance = vDistance.Norm();
        float fMoveDistance = 1000.f * DT;
        if (fDistance <= fMoveDistance)
        {
            m_tInfo.vPoint = m_vIdlePoint;
            RequestChange(GOLEM_HAND_STATE::IDLE);
        }
        else 
            m_tInfo.vPoint += vDistance.Normalize() * fMoveDistance;
        break;
    }
    default:
        break;
    }

}

void CGolemHand::Attack()
{
    if (nullptr == m_pTarget)
        return;

    if (m_eCurState == GOLEM_HAND_STATE::SLAM)
    {
        if (m_dStateTime >= m_dSlamTime)
        {
            VEC vRectSize{ 150.f, 150.f };
            SetRect(&m_vecAtkRect[toUType(GOLEM_HAND_ATK_RECT::SLAM)], m_vSlamPoint.fX - vRectSize.fX * 0.5f, m_vSlamPoint.fY - vRectSize.fY * 0.5f, m_vSlamPoint.fX + vRectSize.fX * 0.5f, m_vSlamPoint.fY + vRectSize.fY * 0.5f);

            // 이펙트 생성
            VEC vRenderPoint = m_tInfo.vPoint;
            vRenderPoint.fY += 100.f;
            CEffectMgr::GetInstance()->CreateEffect(L"Golem_Slam", vRenderPoint, EFTMGR_FIXED | EFTMGR_SCROLL | EFTMGR_IMAGE, 0.f, 0.1f);
        }
    }

    if (m_eCurState == GOLEM_HAND_STATE::LASER)
    {
        VEC vRectSize{ 416.f * PIXEL_SCALE, 80.f };
        if (m_bLeft)
        {
            SetRect(&m_vecAtkRect[toUType(GOLEM_HAND_ATK_RECT::LASER)], 
                m_tInfo.vPoint.fX, 
                m_tInfo.vPoint.fY - vRectSize.fY * 0.5f,
                m_tInfo.vPoint.fX + vRectSize.fX,
                m_tInfo.vPoint.fY + vRectSize.fY * 0.5f);
        }
        else
        {
            SetRect(&m_vecAtkRect[toUType(GOLEM_HAND_ATK_RECT::LASER)],
                m_tInfo.vPoint.fX - vRectSize.fX,
                m_tInfo.vPoint.fY - vRectSize.fY * 0.5f,
                m_tInfo.vPoint.fX,
                m_tInfo.vPoint.fY + vRectSize.fY * 0.5f);
        }
    }
}

void CGolemHand::SetDirection(bool bLeft)
{
	m_bLeft = bLeft;
    if (m_bLeft)
    {
        m_pFrameKey = L"GolemHand_LEFT";
        m_pLaserFrameKey = L"Golem_Laser_LEFT";
    }
    else
    {
        m_pFrameKey = L"GolemHand_RIGHT";
        m_pLaserFrameKey = L"Golem_Laser_RIGHT";
    }
}

void CGolemHand::StartSlam()
{
    // 리턴 상태일 때는 상관없는데, 슬램 중일 때는 또 요청받아도 변화없게 함
    if (m_eCurState == GOLEM_HAND_STATE::SLAM_READY || m_eCurState == GOLEM_HAND_STATE::SLAM)
        return; 

    // 레이저 상태일 때는 슬램 불가능
    if (m_eCurState == GOLEM_HAND_STATE::LASER_READY || m_eCurState == GOLEM_HAND_STATE::LASER_CREATE || m_eCurState == GOLEM_HAND_STATE::LASER || m_eCurState == GOLEM_HAND_STATE::LASER_END)
        return;

    RequestChange(GOLEM_HAND_STATE::SLAM_READY);
    
    m_vSlamReadyStartPoint = m_tInfo.vPoint;
    VEC vEffectPoint = m_tInfo.vPoint + VEC{ 0.f, 280.f };
    CEffectMgr::GetInstance()->CreateEffect( L"Golem_Targeting",vEffectPoint, EFTMGR_FOLLOW | EFTMGR_IMAGE | EFTMGR_SCROLL, 0.f, m_dSlamReadyTime / 20.f, this);
}

void CGolemHand::StartLaser(VEC vStartPoint, VEC vEndPoint)
{
    m_vLaserReadyStart = m_tInfo.vPoint;
    m_vLaserStart = vStartPoint;
    m_vLaserEnd = vEndPoint;

    RequestChange(GOLEM_HAND_STATE::LASER_READY);
}

void CGolemHand::Down()
{
}

void CGolemHand::ApplyChange()
{
    if (m_eCurState != m_eNextState)
    {
        for (auto& rect : m_vecAtkRect)
            SetRectEmpty(&rect);

        switch (m_eNextState)
        {
        case GOLEM_HAND_STATE::IDLE:
            SetFrame(0, 0, 0, INT_MAX);
            break;
        case GOLEM_HAND_STATE::SLAM_READY:
            break;
        case GOLEM_HAND_STATE::SLAM:
            m_vSlamStartPoint = m_tInfo.vPoint;
            break;
        case GOLEM_HAND_STATE::RETURN:
            SetFrame(0, 0, 0, INT_MAX);
            break;
        case GOLEM_HAND_STATE::LASER_READY:
            SetFrame(0, 0, 0, INT_MAX);
            break;
        case GOLEM_HAND_STATE::LASER_CREATE:
            m_tInfo.vPoint = m_vLaserStart;

            SetFrame(0, 5, 0, m_dLaserCreateTime / 6.);
            SetLaserFrame(0, 5, 0, m_dLaserCreateTime / 6.);
            CSoundMgr::GetInstance()->PlaySound(L"GolemLaser.wav", CHANNEL_GROUPID::SFX, 0.2f);
            break;
        case GOLEM_HAND_STATE::LASER:
            SetFrame(5, 11, 0, 0.08);
            SetLaserFrame(6, 11, 0, 0.06);
            break;
        case GOLEM_HAND_STATE::LASER_END:
            SetFrame(12, 17, 0, m_dLaserEndTime / 6.);
            SetLaserFrame(12, 17, 0., m_dLaserEndTime / 6.);
            break;
        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}

void CGolemHand::UpdateFrame()
{
    m_tFrame.dFrameElapsedTime += DT;

    if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
    {
        ++m_tFrame.iStart;
        m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

        if (m_tFrame.iStart > m_tFrame.iEnd)
        {
            switch (m_eCurState)
            {
            case GOLEM_HAND_STATE::LASER_CREATE:
            case GOLEM_HAND_STATE::LASER_END:
                m_tFrame.iStart = m_tFrame.iEnd;
                break;

            case GOLEM_HAND_STATE::LASER:
                m_tFrame.iStart = 5;
                break;

            default:
                m_tFrame.iStart = 0;
                break;
            }
        }
    }

    // 레이저 프레임 업데이트
    UpdateLaserFrame();
}

void CGolemHand::UpdateLaserFrame()
{
    // 레이저 렌더링 관련이 아니면 리턴
    if (m_eCurState != GOLEM_HAND_STATE::LASER_CREATE && m_eCurState != GOLEM_HAND_STATE::LASER && m_eCurState != GOLEM_HAND_STATE::LASER_END)
        return;

    m_tLaserFrame.dFrameElapsedTime += DT;

    if (m_tLaserFrame.dFrameSpeed <= m_tLaserFrame.dFrameElapsedTime)
    {
        ++m_tLaserFrame.iStart;
        m_tLaserFrame.dFrameElapsedTime -= m_tLaserFrame.dFrameSpeed;

        if (m_tLaserFrame.iStart > m_tLaserFrame.iEnd)
        {
            switch (m_eCurState)
            {
            case GOLEM_HAND_STATE::LASER_CREATE:
                m_tLaserFrame.iStart = m_tLaserFrame.iEnd;
                break;

            case GOLEM_HAND_STATE::LASER:
                m_tLaserFrame.iStart = 6;
                break;

            case GOLEM_HAND_STATE::LASER_END:
                m_tLaserFrame.iStart = m_tLaserFrame.iEnd;
                break;

            default:
                break;
            }
        }
    }
}

void CGolemHand::SetLaserFrame(int iStart, int iEnd, int iMotion, double dFrameSpeed)
{
    m_tLaserFrame = {iStart, iEnd, iMotion, dFrameSpeed, 0.};
}

void CGolemHand::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
    if (nullptr == m_pOwner)
        return;

    if (!m_pOwner->TryHitDamage(iDamage, pObj, eHit, bCritical))
        return;

    CreateHitEffect(iDamage, pObj, eHit, bCritical);

    m_bHit = true;
    m_dHitElapseTime = 0.;
}
