#include "pch.h"
#include "CStage1.h"

#include "CPlayer.h"
#include "CGargoyle.h"

#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"

void CStage1::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::STAGE1);
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

	// UI Show
	CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);

	InitializeRooms();
	Init_CreateObj();
	Init_LoadImg(L"../Resource/Image/Stage/Stage01.png");
}

void CStage1::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStage1::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
	CCollisionMgr::CollisionRoom(CObjMgr::GetInstance()->GetPlayer(), this);
	CCollisionMgr::CollisionStair(CObjMgr::GetInstance()->GetPlayer(), m_rcStair, SCENEID::LIB_LOADING);
}

void CStage1::Render(Graphics* pGraphics)
{
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	VEC vViewStart = vScroll * -1;

	int srcLeft = max(0, vViewStart.fX);
	int srcTop = max(0, vViewStart.fY);
	int srcRight = min(TILECX * TILEX, vViewStart.fX + WINCX);
	int srcBottom = min(TILECY * TILEY, vViewStart.fY + WINCY);

	int copyWidth = srcRight - srcLeft;
	int copyHeight = srcBottom - srcTop;

	int dstX = srcLeft - vViewStart.fX;
	int dstY = srcTop - vViewStart.fY;

	HDC hBackDC = pGraphics->GetHDC();

	TransparentBlt(
		hBackDC,
		dstX,
		dstY,
		copyWidth,
		copyHeight,
		m_hMapDC,
		srcLeft,
		srcTop,
		copyWidth,
		copyHeight,
		RGB(255, 0, 255)
	);

	pGraphics->ReleaseHDC(hBackDC);


	// ReleaseHDC 이후부터 GDI+ 사용
	CTileMgr::GetInstance()->Render(pGraphics);
	CObjMgr::GetInstance()->Render(pGraphics);

#ifdef _DEBUG

	Pen roomPen(Color(255, 255, 0, 0), 2.f);

	for (const ROOM_INFO& room : m_vecRooms)
	{
		const RECT& rc = room.rcTrigger;

		pGraphics->DrawRectangle(
			&roomPen,
			RectF(
				rc.left + vScroll.fX,
				rc.top + vScroll.fY,
				static_cast<float>(rc.right - rc.left),
				static_cast<float>(rc.bottom - rc.top)
			)
		);
	}

	pGraphics->DrawRectangle(
		&roomPen,
		RectF(
			m_rcStair.left + vScroll.fX,
			m_rcStair.top + vScroll.fY,
			static_cast<float>(m_rcStair.right - m_rcStair.left),
			static_cast<float>(m_rcStair.bottom - m_rcStair.top)
		)
	);
#endif // _DEBUG
}

void CStage1::Release()
{
}

void CStage1::Init_CreateObj()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(360.f, 7600.f));
	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
}

void CStage1::InitializeRooms()
{
	ROOM_INFO room1, room2, room3;
	room1.rcTrigger = { 200, 4000, 1400, 4600 };
	room2.rcTrigger = { 2300, 2500, 3500, 4800 };
	room3.rcTrigger = { 3150, 5200, 5600, 7250 };

	m_vecRooms.push_back(room1);
	m_vecRooms.push_back(room2);
	m_vecRooms.push_back(room3);

	SetRect(&m_rcStair, 4700, 6700, 5000, 7000);
}

void CStage1::SpawnMonster(int iRoomIdx)
{
	switch (iRoomIdx)
	{
	case 0:
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(600, 4279));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(1000, 4279));
		break;
	case 1:
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(2300, 3900));
		break;
	case 2:
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(4600, 5700));
		break;
	}
}
