#include "pch.h"
#include "CStage1.h"

#include "CPlayer.h"
#include "CBaba.h"
#include "CGargoyle.h"
#include "CGolemCow.h"
#include "CFluffy.h"
#include "CSephirite.h"
#include "CDummy.h"
#include "CAnvil.h"
#include "CInventoryUI.h"

#include "CAbstractFactory.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"
#include "CSoundMgr.h"

CStage1::CStage1()
{
}

CStage1::~CStage1()
{
	Release();
}

void CStage1::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::STAGE1);
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

	Init_CreateObj();

	InitializeRooms();

	Init_LoadImg(L"../Resource/Image/Stage/Stage01.png");
	Init_BGM(L"DugeonLibrary_Field.wav", 0.3f);
}

void CStage1::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStage1::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();

	HotKey();

	// 전투 방 충돌 
	HandleCollisionBattleRoom();
	// 계단 충돌
	HandleCollisionStair();
	// 전투 종료 시 DropItem 빨아오기
	EndBattle();
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

	// 영역 그리기
	Pen areaPen(Color{ 255, 70, 150, 210 }, 5.f);
	RectF rcStair{ m_rcStair.left + vScroll.fX, m_rcStair.top + vScroll.fY, static_cast<float>(m_rcStair.right - m_rcStair.left), static_cast<float>(m_rcStair.bottom - m_rcStair.top) };
	pGraphics->DrawRectangle(&areaPen, rcStair);

	// ReleaseHDC 이후부터 GDI+ 사용
	CTileMgr::GetInstance()->Render(pGraphics);
	CObjMgr::GetInstance()->Render(pGraphics);

#ifdef _DEBUG

	Pen roomPen(Color(255, 255, 0, 0), 2.f);

	for (const ROOM_INFO& room : m_vecRooms)
	{
		const RECT& rc = room.rcTrigger;
		pGraphics->DrawRectangle(&roomPen, RectF(rc.left + vScroll.fX, rc.top + vScroll.fY, static_cast<float>(rc.right - rc.left),static_cast<float>(rc.bottom - rc.top)));
	}
	pGraphics->DrawRectangle(&roomPen,RectF( m_rcStair.left + vScroll.fX, m_rcStair.top + vScroll.fY, static_cast<float>(m_rcStair.right - m_rcStair.left), static_cast<float>(m_rcStair.bottom - m_rcStair.top)));
#endif // _DEBUG
}

void CStage1::Release()
{
}

void CStage1::Init_CreateObj()
{
	CPlayer* pPlayer = static_cast<CPlayer*>(CAbstractFactory<CPlayer>::CreateObj(360.f, 7600.f));
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, pPlayer);
	CObjMgr::GetInstance()->AddObject(OBJID::DUMMY, CAbstractFactory<CDummy>::CreateObj(1475.f, 6590.f));

	CMouse* pMouse = CObjMgr::GetInstance()->GetMouse();
	pMouse->SetHoverItem(nullptr);
	pMouse->SetHoverReferItem({ -1, ITEM_TYPE::END, ITEM_SOURCE::END });
	pMouse->SetDragReferItem({ -1, ITEM_TYPE::END, ITEM_SOURCE::END });

	CInventoryUI* pInventoryUI = static_cast<CInventoryUI*>(CUIMgr::GetInstance()->GetUI(UIID::INVENTORY));
	pInventoryUI->SetInventory(pPlayer->GetInventory());

	CCameraMgr::GetInstance()->SetCameraTarget(pPlayer);
}

void CStage1::InitializeRooms()
{
	ROOM_INFO room1, room2, room3;
	room1.rcTrigger = { 200, 4000, 1400, 4600 };
	room2.rcTrigger = { 2200, 2500, 3550, 4900 };
	room3.rcTrigger = { 3150, 5100, 5380, 6200 };

	m_vecRooms.push_back(room1);
	m_vecRooms.push_back(room2);
	m_vecRooms.push_back(room3);

	SetRect(&m_rcStair, 4718, 6718, 4957, 6960);
}

void CStage1::SpawnMonster(int iRoomIdx)
{
	switch (iRoomIdx)
	{
	case 0:
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(470.f, 4230.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGolemCow>::CreateObj(790.f, 4390.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(1130.f, 4230.f));
		break;
	case 1:
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(2390.f, 3330.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGolemCow>::CreateObj(2890.f, 4170.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(3150.f, 4330.f));
		break;
	case 2:
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(3940.f, 5490.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CFluffy>::CreateObj(4720.f, 5490.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGolemCow>::CreateObj(3940.f, 5870.f));
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CGargoyle>::CreateObj(4720.f, 5870.f));
		break;
	}
}