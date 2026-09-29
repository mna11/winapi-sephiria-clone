#include "pch.h"
#include "CInventorySlotUI.h"
#include "CItem.h"

#include "CMouse.h"

#include "CImgMgr.h"

CInventorySlotUI::CInventorySlotUI()
	: m_pItem(nullptr), m_bCol(false)
{
}

CInventorySlotUI::~CInventorySlotUI()
{
	Release();
}

void CInventorySlotUI::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	m_tInfo = { 0.f, 0.f, 32.f * m_fUIScale, 32.f * m_fUIScale };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 5;      // UI 중에 최강
}

int CInventorySlotUI::Update()
{
	__super::UpdateRect();
	return NOEVENT;
}

void CInventorySlotUI::LateUpdate()
{
}

void CInventorySlotUI::Render(Graphics* pGraphics)
{
	// 배경 그리기
	Image* pInventorySlotImg = CImgMgr::GetInstance()->FindImg(L"Inventory_Slot");
	if (nullptr == pInventorySlotImg)
		return;

	VEC vCellSize = { 32.f, 32.f };
	VEC vImgSize = vCellSize * m_fUIScale;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pInventorySlotImg, rcDest,
		(m_bCol ? vCellSize.fX : 0),
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	if (nullptr == m_pItem)
		return;

	// 아이템 그리기
	Image* pItemImg = CImgMgr::GetInstance()->FindImg(m_pItem->GetItemInfo().strImg.c_str());
	vCellSize = { 32.f, 32.f };
	vImgSize = vCellSize * m_fUIScale;
	rcDest = {	m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
				m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
				vImgSize.fX,
				vImgSize.fY };

	pGraphics->DrawImage(
		pItemImg, rcDest,
		0,
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);
}

void CInventorySlotUI::Release()
{
}
