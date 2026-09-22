#include "pch.h"
#include "CStage1.h"

#include "CPlayer.h"

#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CCameraMgr.h"
#include "CImgMgr.h"
#include "CKeyMgr.h"
#include "CTileMgr.h"
#include "CUIMgr.h"

void CStage1::Initialize()
{
	CTileMgr::GetInstance()->LoadTile(SCENEID::STAGE1);

	// UI Show
	CUIMgr::GetInstance()->ShowUI(UIID::BASIC_INFO);

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
}

void CStage1::Release()
{
}

void CStage1::Init_CreateObj()
{
	CObjMgr::GetInstance()->AddObject(OBJID::PLAYER, CAbstractFactory<CPlayer>::CreateObj(360.f, 7600.f));
	CCameraMgr::GetInstance()->SetCameraTarget(CObjMgr::GetInstance()->GetPlayer());
}
