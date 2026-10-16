#include "pch.h"
#include "CGolem.h"
#include "CGolemMissile.h"

#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CTimeMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CUIMgr.h"
#include "CSoundMgr.h"
#include "CEffectMgr.h"

CGolemMissile::CGolemMissile()
    : CState(GOLEM_MISSILE_STATE::END, GOLEM_MISSILE_STATE::DELAY),
    m_dStateTime(0.), m_dWarningTime(0.), m_dExplosionTime(0.), m_dEndTime(0.), m_dStartDelay(0.)
{
    m_vecAtkRect.resize(toUType(GOLEM_MISSLE_ATK_RECT::END));
}

CGolemMissile::~CGolemMissile()
{
	Release();
}

void CGolemMissile::Initialize()
{
	m_tInfo = { 0.f, 0.f, 0.f, 0.f };

	m_eRender = RENDERID::GAMEOBJECT;
	m_iRenderLayer = 1;

	m_tStat = { 0, 0, 15 };

    m_dWarningTime = 1.0;
    m_dExplosionTime = 1.5;

    m_dEndTime = 0.1;

    m_iLeaf = 0;
    m_iExp = 0;
}

int CGolemMissile::Update()
{
    if (m_bDead)
        return DEAD;

    ApplyChange();
    UpdateTime();
    Attack();

	__super::UpdateRect();
	__super::UpdateFrame();
	return NOEVENT;
}

void CGolemMissile::LateUpdate()
{
}

void CGolemMissile::Render(Graphics* pGraphics)
{
#ifdef _DEBUG
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    // 공격 충돌 박스 렌더링
    SolidBrush BlackBrush(Color(255, 0, 0, 0));
    for (int i = 0; i < toUType(GOLEM_MISSLE_ATK_RECT::END); ++i)
    {
        pGraphics->FillRectangle(&BlackBrush, (int)(m_vecAtkRect[i].left + vScroll.fX),
            (int)(m_vecAtkRect[i].top + vScroll.fY),
            (int)(m_vecAtkRect[i].right - m_vecAtkRect[i].left),
            (int)(m_vecAtkRect[i].bottom - m_vecAtkRect[i].top));
    }
#endif // _DEBUG
}

void CGolemMissile::Release()
{
}

void CGolemMissile::ApplyChange()
{
    if (m_eCurState != m_eNextState)
    {
        switch (m_eNextState)
        {
        case GOLEM_MISSILE_STATE::DELAY:
        {
            break;
        }
        case GOLEM_MISSILE_STATE::WARNING:
        {
            CEffectMgr::GetInstance()->CreateEffect(L"Golem_Alert_Missle", m_tInfo.vPoint, EFTMGR_IMAGE | EFTMGR_FIXED | EFTMGR_SCROLL, 0.f, m_dExplosionTime / 18.f);
            break;
        }
        case GOLEM_MISSILE_STATE::EXPLOSION:
        {
            VEC vOffset{ 0, -200.f };
            CEffectMgr::GetInstance()->CreateEffect(L"Golem_Missile_D", m_tInfo.vPoint + vOffset, EFTMGR_IMAGE | EFTMGR_FIXED | EFTMGR_SCROLL, 0.f, 0.05f);
            break;
        }
        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}

void CGolemMissile::UpdateTime()
{
    if (nullptr == m_pTarget)
        return;

    m_dStateTime += DT;

    switch (m_eCurState)
    {
    case GOLEM_MISSILE_STATE::DELAY:
    {
        if (m_dStateTime >= m_dStartDelay)
            RequestChange(GOLEM_MISSILE_STATE::WARNING);
        break;
    }
    case GOLEM_MISSILE_STATE::WARNING:
    {
        if (m_dStateTime >= m_dWarningTime)
            RequestChange(GOLEM_MISSILE_STATE::EXPLOSION);
        break;
    }
    case GOLEM_MISSILE_STATE::EXPLOSION:
    {
        if (m_dStateTime >= m_dEndTime)
            m_bDead = true;
        break;
    }
    default:
        break;
    }
}

void CGolemMissile::Move()
{
}

void CGolemMissile::Attack()
{
    if (m_eCurState != GOLEM_MISSILE_STATE::EXPLOSION)
        return;

    if (m_dExplosionTime >= m_dStateTime)
    {
        VEC vRectSize{ 300.f, 150.f };

        SetRect(&m_vecAtkRect[toUType(GOLEM_MISSLE_ATK_RECT::MISSILE)], m_tInfo.vPoint.fX - vRectSize.fX * 0.5f, m_tInfo.vPoint.fY - vRectSize.fY * 0.5f, m_tInfo.vPoint.fX + vRectSize.fX * 0.5f, m_tInfo.vPoint.fY + vRectSize.fY * 0.5f);
    }
}
