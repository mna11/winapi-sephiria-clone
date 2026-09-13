#include "pch.h"
#include "CTile.h"
#include "CImgMgr.h"
#include "CCameraMgr.h"

CTile::CTile() : m_iDrawID(0), m_iOption(0)
{
}

CTile::~CTile()
{
    Release();
}

void CTile::Initialize()
{
    m_tInfo.vSize.fX = TILECX;
    m_tInfo.vSize.fY = TILECY;
}

int CTile::Update()
{

    __super::UpdateRect();

    return NOEVENT;
}

void CTile::LateUpdate()
{
}

void CTile::Render(Graphics* pGraphics)
{
    Image* pImg = CImgMgr::GetInstance()->FindImg(L"Tile");
    if (nullptr == pImg)
        return;

    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    pGraphics->DrawImage(
        pImg, 
        int(m_tRect.left + vScroll.fX),
        int(m_tRect.top + vScroll.fY),
        TILECX * m_iDrawID, 0, TILECX, TILECY,
        UnitPixel
        );
}

void CTile::Release()
{
}