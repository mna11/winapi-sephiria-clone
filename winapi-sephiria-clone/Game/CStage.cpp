#include "pch.h"
#include "CStage.h"
#include "CPlayer.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"

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

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Background/test.png", L"Ground");
}

void CStage::Update()
{
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

	int iScrollX = (int)CCameraMgr::GetInstance()->GetScrollX();
	int iScrollY = (int)CCameraMgr::GetInstance()->GetScrollY();

	pGraphics->DrawImage(
		pGround,
		-iScrollX,
		-iScrollY,
		0, 0, 657, 368,
		UnitPixel
	);

	CObjMgr::GetInstance()->Render(pGraphics);
}

void CStage::Release()
{
}
