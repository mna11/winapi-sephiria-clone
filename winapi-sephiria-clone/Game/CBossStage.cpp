#include "pch.h"
#include "CBossStage.h"

#include "CPlayer.h"
#include "CBossErma.h"
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

	// UI Show
	CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);

	Init_CreateObj();
    Init_LoadImg(L"../Resource/Image/Stage/BossStage.png");
}

void CBossStage::Update()
{
	CObjMgr::GetInstance()->Update();
}

void CBossStage::LateUpdate()
{
	CObjMgr::GetInstance()->LateUpdate();
}

void CBossStage::Render(Graphics* pGraphics)
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
}

void CBossStage::Release()
{
}

void CBossStage::Init_CreateObj()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(412.f, 1024.f));
	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());

	// 보스 추가하기
	CBossErma* pBoss = static_cast<CBossErma*>(
		CAbstractFactory<CBossErma>::CreateObj(3960.f, 1100.f));

	CObjMgr::GetInstance()->AddObject(OBJID::MONSTER, pBoss);
	pBoss->InitializeParts();
}
