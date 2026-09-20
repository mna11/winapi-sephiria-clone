#include "pch.h"
#include "CBossStage.h"

#include "CPlayer.h"
#include "CErma.h"
#include "CFluffy.h"
#include "CMonster.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"

CBossStage::CBossStage()
{
}

CBossStage::~CBossStage()
{
	Release();
}

void CBossStage::Initialize()
{
	CTileMgr::GetInstance()->Initialize();
	CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);

	Init_CreateObj();
	Init_InsertImg();
}

void CBossStage::Update()
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

void CBossStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();

	if (CObjMgr::GetInstance()->ObjEmpty(OBJID::MONSTER))
		CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CFluffy>::CreateObj(1700.f, 1200.f));
}

void CBossStage::Render(Graphics* pGraphics)
{
	Image* pGround = CImgMgr::GetInstance()->FindImg(L"Map");
	if (nullptr == pGround)
		return;

	VEC vScroll = CCameraMgr::GetInstance()->GetScroll();

	// 현재 화면이 바라보는 월드 좌표
	float fViewLeft = -vScroll.fX;
	float fViewTop = -vScroll.fY;
	float fViewRight = fViewLeft + WINCX;
	float fViewBottom = fViewTop + WINCY;

	// 이미지 범위 안으로 제한
	float fSrcLeft = std::clamp(fViewLeft, 0.f, 8400.f);
	float fSrcTop = std::clamp(fViewTop, 0.f, 8400.f);
	float fSrcRight = std::clamp(fViewRight, 0.f, 8400.f);
	float fSrcBottom = std::clamp(fViewBottom, 0.f, 8400.f);

	float fDrawWidth = fSrcRight - fSrcLeft;
	float fDrawHeight = fSrcBottom - fSrcTop;

	if (0.f < fDrawWidth && 0.f < fDrawHeight)
	{
		RectF destRect{
			fSrcLeft + vScroll.fX,
			fSrcTop + vScroll.fY,
			fDrawWidth,
			fDrawHeight
		};

		pGraphics->DrawImage(
			pGround,
			destRect,
			fSrcLeft,
			fSrcTop,
			fDrawWidth,
			fDrawHeight,
			UnitPixel
		);
	}

	CTileMgr::GetInstance()->Render(pGraphics);
	CObjMgr::GetInstance()->Render(pGraphics);
}

void CBossStage::Release()
{
}

void CBossStage::Init_CreateObj()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(412.f, 1024.f));
	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, CAbstractFactory<CErma>::CreateObj(4000.f, 1300.f));
}

void CBossStage::Init_InsertImg()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Stage/Map3.png", L"Map");
}