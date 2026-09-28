#include "pch.h"
#include "CErmaBody.h"

#include "CBossErma.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"

CErmaBody::CErmaBody()
    : m_pOwner(nullptr),
    m_bRemoveRequested(false)
{
}

CErmaBody::~CErmaBody()
{
    Release();
}

void CErmaBody::Initialize()
{
    m_tInfo = { 0.f, 0.f, 260.f, 220.f };
    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 2;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Body/BigGolem_Body.png",
        L"Erma_Golem_Body");
}

int CErmaBody::Update()
{
    if (m_bDead || m_bRemoveRequested)
        return DEAD;

    __super::UpdateRect();
    return NOEVENT;
}

void CErmaBody::LateUpdate()
{
}

void CErmaBody::Render(Graphics* pGraphics)
{
    Image* pBody = CImgMgr::GetInstance()->FindImg(L"Erma_Golem_Body");
    if (pBody == nullptr)
        return;

    constexpr float CELL_X = 113.f;
    constexpr float CELL_Y = 55.f;
    const int iFrame = (m_pOwner != nullptr &&
        m_pOwner->GetState() == BOSS_ERMA_STATE::EXPOSED) ? 1 : 0;
    const VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
    const VEC vDrawSize = VEC{ CELL_X, CELL_Y } * PIXEL_SCALE;
    const RectF rcDest{
        m_tInfo.vPoint.fX - vDrawSize.fX * 0.5f + vScroll.fX,
        m_tInfo.vPoint.fY - vDrawSize.fY * 0.5f + vScroll.fY,
        vDrawSize.fX,
        vDrawSize.fY
    };
    ImageAttributes* pImgAttr =
        (m_pOwner != nullptr && m_pOwner->IsHit()) ? &m_imgAttrHit : nullptr;

    pGraphics->DrawImage(
        pBody, rcDest,
        CELL_X * iFrame, 0.f,
        CELL_X, CELL_Y,
        UnitPixel, pImgAttr);

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

void CErmaBody::Release()
{
}

void CErmaBody::SetDamage(int iDamage, CObj* pObj)
{
    if (m_pOwner != nullptr)
        m_pOwner->AddStaggerDamage(iDamage);
}
