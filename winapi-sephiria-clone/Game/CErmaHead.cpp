#include "pch.h"
#include "CErmaHead.h"

#include "CBossErma.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"

CErmaHead::CErmaHead()
    : m_pOwner(nullptr),
    m_bRemoveRequested(false)
{
}

CErmaHead::~CErmaHead()
{
    Release();
}

void CErmaHead::Initialize()
{
    // The body keeps the original golem collision rectangle. The head is a
    // visual-only part so splitting the sprite does not enlarge the hit box.
    m_tInfo = { 0.f, 0.f, 0.f, 0.f };
    m_eRender = RENDERID::GAMEOBJECT;
    m_iRenderLayer = 2;

    CImgMgr::GetInstance()->InsertImg(
        L"../Resource/Image/Monster/Boss/Effect/Head/BigGolem_Head.png",
        L"Erma_Golem_Head");
}

int CErmaHead::Update()
{
    if (m_bDead || m_bRemoveRequested)
        return DEAD;

    __super::UpdateRect();
    return NOEVENT;
}

void CErmaHead::LateUpdate()
{
}

void CErmaHead::Render(Graphics* pGraphics)
{
    Image* pHead = CImgMgr::GetInstance()->FindImg(L"Erma_Golem_Head");
    if (pHead == nullptr)
        return;

    constexpr float CELL_X = 93.f;
    constexpr float CELL_Y = 80.f;
    const int iFrameY = (m_pOwner != nullptr &&
        m_pOwner->GetState() == BOSS_ERMA_STATE::EXPOSED) ? 2 : 0;
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
        pHead, rcDest,
        0.f, CELL_Y * iFrameY,
        CELL_X, CELL_Y,
        UnitPixel, pImgAttr);
}

void CErmaHead::Release()
{
}
