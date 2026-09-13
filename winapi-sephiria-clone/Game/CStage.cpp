#include "pch.h"
#include "CStage.h"
#include "CPlayer.h"
#include "CMonster.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"

CStage::CStage()
{
}

CStage::~CStage()
{
	Release();
}

void CStage::Initialize()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(WINCX >> 1, WINCY >> 1));
	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(WINCX >> 1, WINCY >> 1));
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CMonster>::CreateObj((WINCX >> 1) + 200.f, (WINCY >> 1) + 200.f));

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Background/Map.png", L"Ground");
}

void CStage::Update()
{
	// 테스트용
	if (CKeyMgr::GetInstance()->KeyDown('M'))
		CCameraMgr::GetInstance()->MoveCamera(VEC{ 1000.f, 1000.f });
	if (CKeyMgr::GetInstance()->KeyDown('A'))
		CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	if (CKeyMgr::GetInstance()->KeyDown('S'))
		CCameraMgr::GetInstance()->CameraShaking(5, 2);

	CObjMgr::GetInstance()->Update();
}

void CStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CStage::Render(Graphics* pGraphics)
{
	Image* pGround = CImgMgr::GetInstance()->FindImg(L"Ground");
	if (nullptr == pGround)
		return;
	
	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	pGraphics->DrawImage(
		pGround,
		(int)vScroll.fX,
		(int)vScroll.fY,
		0, 0, 1254, 1254,
		UnitPixel
	);

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CStage::Release()
{
}
