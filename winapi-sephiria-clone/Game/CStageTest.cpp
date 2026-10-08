#include "pch.h"
#include "CStageTest.h"

#include "CPlayer.h"
#include "CBaba.h"
#include "CGargoyle.h"
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

CStageTest::CStageTest()
{
}

CStageTest::~CStageTest()
{
	Release();
}

void CStageTest::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::STAGE_TEST);
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

	Init_CreateObj();

	InitializeRooms();
	Init_LoadImg(L"../Resource/Image/Stage/StageTest.png");

	Init_BGM(L"DugeonLibrary_Field.wav", 0.1f);
}

void CStageTest::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStageTest::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();

	HotKey();

	HandleCollision();
	EndBattle();
}

void CStageTest::Render(Graphics* pGraphics)
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

void CStageTest::Release()
{
}

void CStageTest::Init_CreateObj()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(300.f, 500.f));
	else
		pPlayer->SetPos(300.f, 500.f);
}

void CStageTest::InitializeRooms()
{
	SetRect(&m_rcStair, 750, 360, 300 * PIXEL_SCALE, 300 * PIXEL_SCALE);
}

void CStageTest::SpawnMonster(int iRoomIdx)
{
}

void CStageTest::HandleCollision()
{
	// 전투 방
	HandleCollisionBattleRoom();

	// 계단
	if (CCollisionMgr::CollisionRect(CObjMgr::GetInstance()->GetPlayer()->GetRect(), m_rcStair) && KEY_DOWN('F'))
	{
		CUIMgr::GetInstance()->ShowUI(UIID::STAGE_CHANGE);
		//CSceneMgr::GetInstance()->RequestChange(SCENEID::LIB_LOADING);
	}
}
