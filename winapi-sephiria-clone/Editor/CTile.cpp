#include "pch.h"
#include "CTile.h"
#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTileMgr.h"

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
    return NOEVENT;
}

void CTile::LateUpdate()
{
    
}

void CTile::Render(Graphics* pGraphics)
{
    Image* pTile = CImgMgr::GetInstance()->FindImg(L"LIB_TILE_BG");
    if (nullptr == pTile)
        return;

    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    Render(pGraphics, pTile, vScroll);
}

// 최적화를 위해, TileMgr에서 이미지 객체를 한번만 불러오게 함 - 하는 김에 스크롤도
void CTile::Render(Graphics* pGraphics, Image* pTileImg, VEC& vScroll)
{
    RectF destRect = { m_tRect.left + vScroll.fX, m_tRect.top + vScroll.fY, 
                      TILECX, TILECY };

    int iCellSize = 16;
    pGraphics->DrawImage(
        pTileImg, destRect,
        iCellSize * m_tTile.iTileNumber,
        iCellSize * EnumToInt(m_tTile.eTileType),
        iCellSize,
        iCellSize,
        UnitPixel
    );
}

void CTile::Render(Graphics* pGraphics, Image* pTileImg, VEC& vScroll, ImageAttributes& ImgAttr)
{
    RectF destRect = { m_tRect.left + vScroll.fX, m_tRect.top + vScroll.fY,
                     TILECX, TILECY };

    int iCellSize = 16;

    pGraphics->DrawImage(
        pTileImg, destRect,
        iCellSize * m_tTile.iTileNumber,
        iCellSize * EnumToInt(m_tTile.eTileType),
        iCellSize,
        iCellSize,
        UnitPixel,
        &ImgAttr
    );
}

void CTile::Release()
{
}

