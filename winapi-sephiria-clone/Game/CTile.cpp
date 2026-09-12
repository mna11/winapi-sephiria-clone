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

    int iScrollX = (int)CCameraMgr::GetInstance()->GetScrollX();
    int iScrollY = (int)CCameraMgr::GetInstance()->GetScrollY();
    pGraphics->DrawImage(
        pImg, 
        m_tRect.left + iScrollX,
        m_tRect.top + iScrollY,
        TILECX * m_iDrawID, 0, TILECX, TILECY,
        UnitPixel
        );
}

void CTile::Release()
{
}