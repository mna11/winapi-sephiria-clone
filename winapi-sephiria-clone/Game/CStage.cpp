#include "pch.h"
#include "CStage.h"

#include "CPlayer.h"
#include "CErma.h"
#include "CMonster.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CTileMgr::GetInstance()->Initialize();

	Init_CreateObj();
	Init_InsertImg();
}

void CStage::Update()
{
	// 테스트용
	/*if (CKeyMgr::GetInstance()->KeyDown('M'))
		CCameraMgr::GetInstance()->MoveCamera(VEC{ 1000.f, 1000.f });
	if (CKeyMgr::GetInstance()->KeyDown('A'))
		CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	if (CKeyMgr::GetInstance()->KeyDown('S'))
		CCameraMgr::GetInstance()->CameraShaking(5, 2);*/

	CObjMgr::GetInstance()->Update();
}

void CStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CStage::Render(Graphics* pGraphics)
{
	Image* pGround = CImgMgr::GetInstance()->FindImg(L"Map");
	if (nullptr == pGround)
		return;
	
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();
	RectF destRect = { 0.f + vScroll.fX, 0.f + vScroll.fY,  6400.f, 6400.f };
	
	pGraphics->DrawImage(
		pGround, destRect,
		0.f, 0.f, 6400.f, 6400.f,
		UnitPixel
	);

	CTileMgr::GetInstance()->Render(pGraphics);

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CStage::Release()
{
}

void CStage::Init_CreateObj()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(280.f, 280.f));
	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CErma>::CreateObj(1700.f, 1000.f));
}

void CStage::Init_InsertImg()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Stage/Map.png", L"Map");
}