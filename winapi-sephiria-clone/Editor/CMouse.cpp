#include "pch.h"
#include "CMouse.h"

#include "CImgMgr.h"
#include "CCameraMgr.h"
#include "CTileMgr.h"

CMouse::CMouse()
	: m_eEditState(EDIT_STATE::END), m_pTile(nullptr)
{
}

CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize()
{
	m_eRender = RENDERID::UI;

	__super::UpdateRect();
}

int CMouse::Update()
{
	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_tInfo.vPoint = { float(ptMouse.x), float(ptMouse.y) };

	return NOEVENT;
}

void CMouse::LateUpdate()
{
}

void CMouse::Render(Graphics* pGraphics)
{
	switch (m_eEditState)
	{
	case EDIT_STATE::RECT:
		break;
	case EDIT_STATE::LINE:
		break;
	case EDIT_STATE::TILE:
		HandleEditTileRender(pGraphics);
		break;
	}
}

void CMouse::Release()
{
}

void CMouse::HandleEditTileRender(Graphics* pGraphics)
{
	Image* pTileImg = CImgMgr::GetInstance()->FindImg(L"LIB_TILE_BG");
	if (nullptr == pTileImg || nullptr == m_pTile || !m_pTile->bDraw)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();


	Rect destRect = { int(m_tInfo.vPoint.fX - TILECX * 0.5f), 
					  int(m_tInfo.vPoint.fY - TILECY * 0.5f),
					  TILECX, TILECY };

    int iCellSize = 16;
    ColorMatrix colorMatrix = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.5f, 0.0f,
                                0.0f, 0.0f, 0.0f, 0.0f, 1.0f };

    if (CTileMgr::GetInstance()->GetPreview())
    {
        switch (m_pTile->eTileOption)
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
    }

    ImageAttributes imageAtt;
    imageAtt.SetColorMatrix(&colorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

    pGraphics->DrawImage(
        pTileImg, destRect,
        iCellSize * m_pTile->iTileNumber,
        iCellSize * EnumToInt(m_pTile->eTileType),
        iCellSize,
        iCellSize,
        UnitPixel,
        &imageAtt
    );
}
