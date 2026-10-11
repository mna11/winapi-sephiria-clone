#include "pch.h"
#include "CGolemHead.h"

#include "CAbstractFactory.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CObjMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"

CGolemHead::CGolemHead()
    : CState(GOLEM_HEAD_STATE::END, GOLEM_HEAD_STATE::IDLE),
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
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };

    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 1;
}

int CGolemHead::Update()
{
    if (m_bDead)
        return DEAD;

    __super::UpdateRect();
    __super::UpdateFrame();
    return NOEVENT;
}

void CGolemHead::LateUpdate()
{
}

void CGolemHead::Render(Graphics* pGraphics)
{
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    // 머리
    Image* pHeadImg = CImgMgr::GetInstance()->FindImg(L"GolemHead");
    if (nullptr == pHeadImg)
        return;

    VEC vCellSize{ 93.f, 80.f };
    VEC vImgSize = vCellSize * PIXEL_SCALE;
    VEC vOffset{ vScroll.fX, vScroll.fY };
    RectF DestRect = { m_tInfo.vPoint.fX + vOffset.fX - vImgSize.fX * 0.5f, m_tInfo.vPoint.fY + vOffset.fY - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
    pGraphics->DrawImage(
        pHeadImg, DestRect,
        0, 0, vCellSize.fX, vCellSize.fY,
        UnitPixel
    );
}

void CGolemHead::Release()
{
}

void CGolemHead::UpdateTime()
{
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

            break;

        case GOLEM_HEAD_STATE::SHOOT:

            break;

        default:
            break;
        }

        m_eCurState = m_eNextState;
        m_dStateTime = 0.;
    }
}
