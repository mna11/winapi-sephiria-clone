#include "pch.h"
#include "CGolemHead.h"

#include "CGolem.h"

#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"
#include "CEffectMgr.h"

CGolemHead::CGolemHead()
    : CState(GOLEM_HEAD_STATE::END, GOLEM_HEAD_STATE::EXPOSE),
    m_pOwner(nullptr),
    m_dStateTime(0.0)
{
}

CGolemHead::~CGolemHead()
{
    Release();
}

void CGolemHead::Initialize()
{
    m_tInfo = { 0.f, 0.f, 300.f, 300.f };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 1;
}

int CGolemHead::Update()
{
    if (m_bDead)
        return DEAD;

    ApplyChange();
    UpdateTime();

    UpdateFrame();
    __super::UpdateRect();
    return NOEVENT;
}

void CGolemHead::LateUpdate()
{
}

void CGolemHead::Render(Graphics* pGraphics)
{
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

#ifdef _DEBUG
    // 피격 충돌 박스 렌더링
    SolidBrush WhiteBrush(Color(255, 255, 255, 255));
    pGraphics->FillRectangle(&WhiteBrush, (int)(m_tRect.left + vScroll.fX),
        (int)(m_tRect.top + vScroll.fY),
        (int)m_tInfo.vSize.fX,
        (int)m_tInfo.vSize.fY);
#endif // _DEBUG

    // 머리
    Image* pHeadImg = CImgMgr::GetInstance()->FindImg(L"GolemHead");
    if (nullptr == pHeadImg)
        return;

    VEC vCellSize{ 93.f, 80.f };
    VEC vImgSize = vCellSize * PIXEL_SCALE;
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
        vCellSize.fX * m_tFrame.iStart, vCellSize.fY * m_tFrame.iMotion, vCellSize.fX, vCellSize.fY,
        UnitPixel, pImgAttr
    );
}

void CGolemHead::Release()
{
}

void CGolemHead::UpdateFrame()
{
    m_tFrame.dFrameElapsedTime += DT;
    if (m_tFrame.dFrameSpeed <= m_tFrame.dFrameElapsedTime)
    {
        ++m_tFrame.iStart;
        m_tFrame.dFrameElapsedTime -= m_tFrame.dFrameSpeed;

        if (m_tFrame.iStart > m_tFrame.iEnd)
        {
            if (m_eCurState == GOLEM_HEAD_STATE::RECOVER)
            {
                m_tFrame.iStart = m_tFrame.iEnd;
                RequestChange(GOLEM_HEAD_STATE::IDLE);
            }
            else
            {
                m_tFrame.iStart = 0;
            }
        }
    }
}

void CGolemHead::UpdateTime()
{
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

void CGolemHead::Move()
{
}

void CGolemHead::Attack()
{
}

void CGolemHead::ApplyChange()
{
    if (m_eCurState != m_eNextState)
    {

        switch (m_eNextState)
        {
        case GOLEM_HEAD_STATE::IDLE:
            SetFrame(0, 0, 0, INT_MAX);
            break;
        case GOLEM_HEAD_STATE::SHOOT:
            SetFrame(0, 7, 1, 0.2);
            break;
        case GOLEM_HEAD_STATE::EXPOSE:
            SetFrame(0, 0, 4, INT_MAX);
            break;
        case GOLEM_HEAD_STATE::RECOVER:
            SetFrame(0, 4, 2, 0.2);
            break;
        case GOLEM_HEAD_STATE::DOWN:
            SetFrame(0, 0, 5, INT_MAX);
            break;

        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}

void CGolemHead::HitDamage(int iDamage, CObj* pObj, HIT_SOURCE eHit, bool bCritical)
{
    if (nullptr == m_pOwner)
        return;

    if (!m_pOwner->TryHitDamage(iDamage, pObj, eHit, bCritical))
        return;

    CreateHitEffect(iDamage, pObj, eHit, bCritical);

    // 피해가 적용된 부위만 피격 연출 시작
    m_bHit = true;
    m_dHitElapseTime = 0.;
}