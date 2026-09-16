#include "pch.h"
#include "CEditor.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMouse.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CTimeMgr.h"

CEditor::CEditor()
	: m_eState(EDIT_STATE::TILE)
{
	ZeroMemory(&m_tTile, sizeof(TILE));
}

CEditor::~CEditor()
{
	Release();
}

void CEditor::Initialize()
{
	m_eState = EDIT_STATE::TILE;
	m_tTile = { TILE_TYPE::LIB_FLOOR1, 0, TILE_OPTION::FLOOR, TILE_LAYER::LAYER0 };

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Map/LibraryRoom_Combat_10.png", L"Ground");
	
	m_mapTileMax.insert({ TILE_TYPE::LIB_DUST, 58 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_WALL, 55 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_CRACK_WALL, 55 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_FLOOR0, 36 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_FLOOR1, 48 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_CLIFF, 48 });
	m_mapTileMax.insert({ TILE_TYPE::WOOD_FLOOR, 47 }); 
	m_mapTileMax.insert({ TILE_TYPE::LIB_GRASS, 9 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_BOOKSHELF, 87 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_FURNITURE, 20 });
	m_mapTileMax.insert({ TILE_TYPE::LIB_STONE_WALL, 19 });

	// 마우스 초기화
	CObj* pMouse = CAbstractFactory<CMouse>::CreateObj();
	static_cast<CMouse*>(pMouse)->SetTile(&m_tTile);
	static_cast<CMouse*>(pMouse)->SetEditState(EDIT_STATE::TILE);
	CObjMgr::GetInstance()->AddObject(OBJID::MOUSE, pMouse);

	m_tTile.bDraw = true;
}

void CEditor::Update()
{
	KeyInput();

	CTileMgr::GetInstance()->Update();
	CObjMgr::GetInstance()->Update();
}

void CEditor::LateUpdate()
{
	CTileMgr::GetInstance()->LateUpdate();
	CObjMgr::GetInstance()->LateUpdate();
}

void CEditor::Render(Graphics* pGraphics)
{
	Image* pGround = CImgMgr::GetInstance()->FindImg(L"Ground");
	if (nullptr == pGround)
		return;

	//VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	//Rect destRect = { int(0.f + vScroll.fX), int(0.f + vScroll.fY), int(320.f * PIXEL_SCALE), int(320.f * PIXEL_SCALE) };
	//RECT srcRect = { 0.f, 0.f, 320.f, 320.f };
	/*pGraphics->DrawImage(
		pGround, destRect,
		srcRect.left, srcRect.top, srcRect.right, srcRect.bottom,
		UnitPixel
	);*/

	CTileMgr::GetInstance()->Render(pGraphics);
	CObjMgr::GetInstance()->Render(pGraphics);
	GirdRender(pGraphics);
}

void CEditor::Release()
{
}

void CEditor::KeyInput()
{
	// 카메라 이동
	float fSpeed = 100.f * float(DT);
	if (KEY_PRESS(VK_LEFT))
		CCameraMgr::GetInstance()->AddCameraPos({ -fSpeed, 0.f });
	if (KEY_PRESS(VK_RIGHT))
		CCameraMgr::GetInstance()->AddCameraPos({ fSpeed, 0.f });
	if (KEY_PRESS(VK_UP))
		CCameraMgr::GetInstance()->AddCameraPos({ 0.f, -fSpeed });
	if (KEY_PRESS(VK_DOWN))
		CCameraMgr::GetInstance()->AddCameraPos({ 0.f, fSpeed });

	// 타입 변경
	if (KEY_DOWN('7'))
	{
		m_eState = EDIT_STATE::TILE;
		CObjMgr::GetInstance()->GetMouse()->SetEditState(EDIT_STATE::TILE);
	}
	if (KEY_DOWN('8'))
	{
		m_eState = EDIT_STATE::LINE; 
		CObjMgr::GetInstance()->GetMouse()->SetEditState(EDIT_STATE::LINE);
	}
	if (KEY_DOWN('9'))
	{
		m_eState = EDIT_STATE::RECT;
		CObjMgr::GetInstance()->GetMouse()->SetEditState(EDIT_STATE::RECT);
	}
		

	switch (m_eState)
	{
	case EDIT_STATE::LINE:
		HandleLineInput();
		break;
	case EDIT_STATE::RECT:
		HandleRectInput();
		break;
	case EDIT_STATE::TILE:
		HandleTileInput();
		break;
	default:
		break;
	}
}

void CEditor::HandleRectInput()
{
}

void CEditor::HandleTileInput()
{
	if (KEY_PRESS(VK_LBUTTON))
	{
		const CObj* pMouse = CObjMgr::GetInstance()->GetMouse();
		POINT ptMouse = { (int)pMouse->GetInfo().vPoint.fX, (int)pMouse->GetInfo().vPoint.fY };

		ptMouse.x -= (int)CCameraMgr::GetInstance()->GetScroll().fX;
		ptMouse.y -= (int)CCameraMgr::GetInstance()->GetScroll().fY;

		CTileMgr::GetInstance()->PickingTile(ptMouse, m_tTile);
	}

	// 타일 타입
	if (KEY_DOWN(VK_F1)) {
		m_tTile.eTileType = TILE_TYPE::LIB_DUST;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F2))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_WALL;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F3))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_CRACK_WALL;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F4))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_FLOOR0;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F5))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_FLOOR1;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F6))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_CLIFF;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F7))
	{
		m_tTile.eTileType = TILE_TYPE::WOOD_FLOOR;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F8))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_GRASS;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F9))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_BOOKSHELF;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F10))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_FURNITURE;
		m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_F11))
	{
		m_tTile.eTileType = TILE_TYPE::LIB_STONE_WALL;
		m_tTile.iTileNumber = 0;
	}

	// 타일 번호
	if (KEY_DOWN(VK_OEM_MINUS))
	{
		--m_tTile.iTileNumber;
		if (m_tTile.iTileNumber <= 0)
			m_tTile.iTileNumber = 0;
	}
	if (KEY_DOWN(VK_OEM_PLUS))
	{
		++m_tTile.iTileNumber;
		if (m_tTile.iTileNumber >= m_mapTileMax[m_tTile.eTileType])
			m_tTile.iTileNumber = m_mapTileMax[m_tTile.eTileType];
	}

	// 타일 레이어
	if (KEY_DOWN('1')) {
		m_tTile.eTileLayer = TILE_LAYER::LAYER0;
	}
	if (KEY_DOWN('2'))
	{
		m_tTile.eTileLayer = TILE_LAYER::LAYER1;
	}

	// 타일 옵션
	if (KEY_DOWN('F')) {
		m_tTile.eTileOption = TILE_OPTION::FLOOR;
	}
	if (KEY_DOWN('W'))
	{
		m_tTile.eTileOption = TILE_OPTION::WALL;
	}
	if (KEY_DOWN('A'))
	{
		m_tTile.eTileOption = TILE_OPTION::AIR;
	}
	if (KEY_DOWN('I'))
	{
		m_tTile.eTileOption = TILE_OPTION::INTERACTION;
	}
	
	// 저장
	if (KEY_DOWN('S'))
	{
		CTileMgr::GetInstance()->SaveTile();
		return;
	}

	// 로드
	if (KEY_DOWN('L'))
	{
		CTileMgr::GetInstance()->LoadTile();
		return;
	}

	// 프리뷰
	if (KEY_DOWN('P'))
	{
		CTileMgr::GetInstance()->SetPreview();
	}

	// 지우기
	if (KEY_DOWN('E'))
	{
		m_tTile.bDraw = !m_tTile.bDraw;
	}
}

void CEditor::HandleLineInput()
{
}


void CEditor::GirdRender(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	Pen gridPen(Color(100, 0, 0, 0), 1.f);

	// 세로 줄
	for (int x = 0; x <= TILEX; ++x)
	{
		// (x1, y1)부터 (x2, y2)까지 선 긋기
		pGraphics->DrawLine(
			&gridPen,
			x * TILECX + (int)vScroll.fX, (int)vScroll.fY,
			x * TILECX + (int)vScroll.fX, (TILEY * TILECY + (int)vScroll.fY)
		);
	}

	// 가로 줄
	for (int y = 0; y <= TILEY; ++y)
	{
		pGraphics->DrawLine(
			&gridPen,
			(int)vScroll.fX, y * TILECY + (int)vScroll.fY,
			(TILEX * TILECX) + (int)vScroll.fX, y * TILECY + +(int)vScroll.fY
		);
	}
}

