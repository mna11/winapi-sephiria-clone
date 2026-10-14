#include "pch.h"
#include "CStage3.h"

#include "CPlayer.h"
#include "CBaba.h"
#include "CGargoyle.h"
#include "CFluffy.h"
#include "CGolemCow.h"
#include "CSephirite.h"
#include "CAnvil.h"
#include "CKeyGuide.h"
#include "CFishingUI.h"

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

CStage3::CStage3()
	: m_iPlayerWeaponID(-1)
{
	ZeroMemory(&m_rcFishing, sizeof(RECT));
}

CStage3::~CStage3()
{
	Release();
}

void CStage3::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::STAGE3);
	CTileMgr::GetInstance()->SetInteractionBlocked(false);

	Init_CreateObj();
	InitializeRooms();

	Init_LoadImg(L"../Resource/Image/Stage/Stage03.png");
	Init_BGM(L"DugeonLibrary_Field.wav", 0.3f);
}

void CStage3::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CStage3::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();

	HotKey();

	// 전투 방 충돌 
	HandleCollisionBattleRoom();
	// 계단 충돌
	HandleCollisionStair(); 
	// 낚시 스팟 충돌 
	HandleCollisionFishingSpot();
	// 전투 종료 시 DropItem 빨아오기
	EndBattle();
}

void CStage3::Render(Graphics* pGraphics)
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
	pGraphics->DrawRectangle(&roomPen, RectF(m_rcFishing.left + vScroll.fX, m_rcFishing.top + vScroll.fY, static_cast<float>(m_rcFishing.right - m_rcFishing.left), static_cast<float>(m_rcFishing.bottom - m_rcFishing.top)));
#endif // _DEBUG
}

void CStage3::Release()
{
}

void CStage3::Init_CreateObj()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
	{
		//CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(580.f, 1480.f));
		CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(4200.f, 1300.f));
		CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	}
	else
		CObjMgr::GetInstance()->GetPlayer()->SetPos(VEC{ 580.f, 1480.f });

	// 필드 아이템 
	CSephirite* pArtifactSephirite = static_cast<CSephirite*>(CAbstractFactory<CSephirite>::CreateObj(2550.f, 390.f));
	pArtifactSephirite->SetItemType(ITEM_TYPE::ARTIFACT);
	CSephirite* pStoneTabletSephirite = static_cast<CSephirite*>(CAbstractFactory<CSephirite>::CreateObj(3520.f, 390.f));
	pStoneTabletSephirite->SetItemType(ITEM_TYPE::STONE_TABLET);
	CObjMgr::GetInstance()->AddObject(OBJID::FIELD_ITEM, pArtifactSephirite);
	CObjMgr::GetInstance()->AddObject(OBJID::FIELD_ITEM, pStoneTabletSephirite);
}

void CStage3::InitializeRooms()
{
	ROOM_INFO room1, room2;
	room1.rcTrigger = { 2690, 1025, 4620, 2120 };
	room2.rcTrigger = { 540, 2860, 2000, 4350 };

	m_vecRooms.push_back(room1);
	m_vecRooms.push_back(room2);

	SetRect(&m_rcStair, 1220, 5600, 1420, 5800);
	SetRect(&m_rcFishing, 4100, 1380, 4300, 1580);
}

void CStage3::SpawnMonster(int iRoomIdx)
{
	switch (iRoomIdx)
	{
	case 0:
		//CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CFluffy>::CreateObj(2800.f, 1525.f));
		break;
	case 1:
		break;
	default:
		break;
	}
}

void CStage3::HandleCollisionFishingSpot()
{
	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	CKeyGuide* pKeyGuide = static_cast<CKeyGuide*>(CUIMgr::GetInstance()->GetUI(UIID::KEY_GUIDE));

	if (CCollisionMgr::CollisionRect(pPlayer->GetRect(), m_rcFishing))
	{
		CFishingUI* pFishingUI = static_cast<CFishingUI*>(CUIMgr::GetInstance()->ShowUI(UIID::FISHING));
		
		if (m_iPlayerWeaponID == -1)
		{
			m_iPlayerWeaponID = pPlayer->GetWeaponController()->GetWeaponID();

			if (m_iPlayerWeaponID != 3)
				pPlayer->GetWeaponController()->RequestChange(3);
		}
	}
	else
	{
		CUIMgr::GetInstance()->HideUI(UIID::FISHING);

		if (m_iPlayerWeaponID == -1)
			return;

		pPlayer->GetWeaponController()->RequestChange(m_iPlayerWeaponID);
		m_iPlayerWeaponID = -1;
	}
}
