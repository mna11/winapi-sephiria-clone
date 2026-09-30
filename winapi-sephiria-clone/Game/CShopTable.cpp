#include "pch.h"
#include "CShopTable.h"

#include "CImgMgr.h"

CShopTable::CShopTable()
{
}

CShopTable::~CShopTable()
{
	Release();
}

void CShopTable::Initialize()
{
	m_tInfo = { 50.f, 14.f, 0.f, 0.f};

	m_eRender = RENDERID::UI;
	m_iRenderLayer = 3;
	m_fUIScale = PIXEL_SCALE * 0.5f;
}

int CShopTable::Update()
{
	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CShopTable::LateUpdate()
{
	if (!m_bView)
		return;
}

void CShopTable::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	// 배경 그리기
	Image* pInventoryBaseImg = CImgMgr::GetInstance()->FindImg(L"ShopList");
	if (nullptr == pInventoryBaseImg)
		return;

	VEC vCellSize = { 196.f, 277.f };
	VEC vImgSize = vCellSize * m_fUIScale;
	RectF rcDest = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vImgSize.fX, vImgSize.fY };
	pGraphics->DrawImage(pInventoryBaseImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);
}

void CShopTable::Release()
{
}
