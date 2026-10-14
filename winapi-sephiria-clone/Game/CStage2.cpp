#include "pch.h"
#include "CStage2.h"

#include "CPlayer.h"
#include "CBaba.h"
#include "CGargoyle.h"
#include "CGolemCow.h"
#include "CSephirite.h"
#include "CAnvil.h"

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

CStage2::CStage2()
{
}

CStage2::~CStage2()
{
	Release();
}

void CStage2::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::STAGE2);
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

	Init_CreateObj();

	InitializeRooms();

	Init_LoadImg(L"../Resource/Image/Stage/Stage02.png");
	Init_BGM(L"DugeonLibrary_Field.wav", 0.3f);
}

void CStage2::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStage2::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();

	HotKey();

	HandleCollisionBattleRoom();
	HandleCollisionStair();
	EndBattle();
}

void CStage2::Render(Graphics* pGraphics)
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
		pGraphics->DrawRectangle(&roomPen, RectF(rc.left + vScroll.fX, rc.top + vScroll.fY, static_cast<float>(rc.right - rc.left), static_cast<float>(rc.bottom - rc.top)));
	}
	pGraphics->DrawRectangle(&roomPen, RectF(m_rcStair.left + vScroll.fX, m_rcStair.top + vScroll.fY, static_cast<float>(m_rcStair.right - m_rcStair.left), static_cast<float>(m_rcStair.bottom - m_rcStair.top)));
#endif // _DEBUG
}

void CStage2::Release()
{
}

void CStage2::Init_CreateObj()
{
	if (nullptr == CObjMgr::GetInstance()->GetPlayer())
	{
		CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(1200.f, 1300.f));

		CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	}
	else
		CObjMgr::GetInstance()->GetPlayer()->SetPos({ 1200.f, 1300.f });

	CObjMgr::GetInstance()->AddObject(OBJID::FIELD_ITEM, CAbstractFactory<CAnvil>::CreateObj(1000.f, 1700.f));
	CObjMgr::GetInstance()->AddObject(OBJID::NPC, CAbstractFactory<CBaba>::CreateObj(1500.f, 1200.f));
}

void CStage2::InitializeRooms()
{
	m_vecRooms.clear();

	ROOM_INFO room1{};
	room1.rcTrigger = { 2630, 480, 3920, 2240 };

	ROOM_INFO room2{};
	room2.rcTrigger = { 2520, 3000, 4080, 4320 };

	ROOM_INFO room3{};
	room3.rcTrigger = { 4450, 2560, 5760, 4320 };

	m_vecRooms.push_back(room1);
	m_vecRooms.push_back(room2);
	m_vecRooms.push_back(room3);

	SetRect(&m_rcStair, 5000, 4040, 5300, 4200);
}
void CStage2::SpawnMonster(int iRoomIdx)
{
	switch (iRoomIdx)
	{
	case 0: // 오른쪽 위
		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGargoyle>::CreateObj(2920.f, 1480.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGolemCow>::CreateObj(3560.f, 1400.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGolemCow>::CreateObj(3560.f, 1640.f));
		break;

	case 1: // 아래 가운데
		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGargoyle>::CreateObj(3400.f, 3720.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGargoyle>::CreateObj(3640.f, 3080.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGolemCow>::CreateObj(3320.f, 3960.f));
		break;

	case 2: // 아래 오른쪽
		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGargoyle>::CreateObj(4760.f, 3560.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGargoyle>::CreateObj(5480.f, 3560.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGolemCow>::CreateObj(5480.f, 3000.f));

		CObjMgr::GetInstance()->AddObject(
			OBJID::MONSTER,
			CAbstractFactory<CGolemCow>::CreateObj(5000.f, 3800.f));
		break;
	}
}
