#include "pch.h"
#include "CTile.h"
#include "CImgMgr.h"
#include "CCameraMgr.h"

CTile::CTile()
{
    ZeroMemory(&m_tTile, sizeof(TILE));
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
    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    Color color(0, 0, 0, 0);

    switch (m_tTile.eTileOption)
    {
    case TILE_OPTION::FLOOR:
        break;

    case TILE_OPTION::WALL:
        color = Color(100, 0, 100, 255);   // 파랑
        break;

    case TILE_OPTION::AIR:
        color = Color(100, 255, 0, 0);     // 빨강
        break;

    case TILE_OPTION::INTERACTION:
        color = Color(100, 0, 0, 255);   // 노랑
        break;

    default:
        return;
    }

    SolidBrush brush(color);

    pGraphics->FillRectangle(
        &brush,
        RectF(
            m_tRect.left + vScroll.fX,
            m_tRect.top + vScroll.fY,
            m_tInfo.vSize.fX,
            m_tInfo.vSize.fY
        )
    );
}

void CTile::Release()
{
}