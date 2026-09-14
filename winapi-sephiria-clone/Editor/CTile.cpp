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
    if (!m_tTile.bDraw)
        return NOEVENT;

    __super::UpdateRect();

    return NOEVENT;
}

void CTile::LateUpdate()
{
    if (!m_tTile.bDraw)
        return;
}

void CTile::Render(Graphics* pGraphics)
{
    if (!m_tTile.bDraw)
        return;

    Image* pTile = CImgMgr::GetInstance()->FindImg(L"LIB_TILE_BG");
    if (nullptr == pTile)
        return;

    VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

    RectF destRect = { m_tRect.left + vScroll.fX, m_tRect.top + vScroll.fY, 
                      TILECX, TILECY };

    int iCellSize = 16;
    if (CTileMgr::GetInstance()->GetPreview())
    {
        ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

        switch (m_tTile.eTileOption)
        {
        case TILE_OPTION::WALL:
            colorMatrix.m[4][2] = 0.5f;
            break;
        case TILE_OPTION::INTERACTION:
            colorMatrix.m[4][1] = 0.5f;
            break;
        case TILE_OPTION::AIR:
            colorMatrix.m[4][0] = 0.5f;
            break;
        case TILE_OPTION::FLOOR:
        default:
            break;
        }
        ImageAttributes imageAtt;
        imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);
        pGraphics->DrawImage(
            pTile, destRect,
            iCellSize * m_tTile.iTileNumber,
            iCellSize * EnumToInt(m_tTile.eTileType),
            iCellSize,
            iCellSize,
            UnitPixel,
            &imageAtt
        );
    }
    else
    {
        pGraphics->DrawImage(
            pTile, destRect,
            iCellSize * m_tTile.iTileNumber,
            iCellSize * EnumToInt(m_tTile.eTileType),
            iCellSize,
            iCellSize,
            UnitPixel
        );
    }
}

void CTile::Release()
{
}