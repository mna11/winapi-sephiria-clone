#include "pch.h"
#include "CEnd.h"

#include "CEffectMgr.h"
#include "CFontMgr.h"
#include "CImgMgr.h"
#include "CTimeMgr.h"
#include "CUIMgr.h"
#include "CSceneMgr.h"

void CEnd::Initialize()
{
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Start/Logo.png", L"Logo");
}

void CEnd::Update()
{
	m_dEndTime -= DT;
}

void CEnd::LateUpdate()
{
	if (m_dEndTime <= 0)
		DestroyWindow(g_hWnd);
}

void CEnd::Render(Graphics* pGraphics)
{
	HDC hBackDC = pGraphics->GetHDC();

	RECT rc{ 0, 0, WINCX, WINCY };
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hBackDC, &rc, brush);
	DeleteObject(brush);
	pGraphics->ReleaseHDC(hBackDC);

	VEC vCellSize{ 237.f, 76.f };
	VEC vImgSize = vCellSize * PIXEL_SCALE * 0.5f;
	RectF DestRect = { (WINCX >> 1) - vImgSize.fX * 0.5f, (WINCY >> 1) - vImgSize.fY * 0.5f, vImgSize.fX, vImgSize.fY };
	Image* pImg = CImgMgr::GetInstance()->FindImg(L"Logo");
	pGraphics->DrawImage(
		pImg, DestRect,
		0, 0, vCellSize.fX, vCellSize.fY,
		UnitPixel
	);
}

void CEnd::Release()
{
}

void CEnd::Init_CreateObj()
{
}
