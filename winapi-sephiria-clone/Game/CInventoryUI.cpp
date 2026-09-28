#include "pch.h"
#include "CInventoryUI.h"

#include "CImgMgr.h"

CInventoryUI::CInventoryUI()
{
}

CInventoryUI::~CInventoryUI()
{
	Release();
}

void CInventoryUI::Initialize()
{
	m_tInfo = { 100.f, 100.f, 0.f, 0.f }; 

	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventoryBase.png", L"Inventory_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot.png", L"Inventory_Slot");

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 5;      // UI 중에 최강
}

int CInventoryUI::Update()
{
	if (!m_bView)
		return NOEVENT;

	return NOEVENT;
}

void CInventoryUI::LateUpdate()
{
	if (!m_bView)
		return;
}

void CInventoryUI::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	Image* pInventoryBaseImg = CImgMgr::GetInstance()->FindImg(L"Inventory_Base");
	Image* pInventorySlotImg = CImgMgr::GetInstance()->FindImg(L"Inventory_Slot");
	if (nullptr == pInventoryBaseImg || nullptr == pInventorySlotImg)
		return;

	VEC vCellSize = { 282.f, 206.f };
	VEC vDrawSize = vCellSize;

	RectF rcDest = { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY, vDrawSize.fX, vDrawSize.fY};
	pGraphics->DrawImage(pInventoryBaseImg, rcDest, 0, 0, vCellSize.fX, vCellSize.fY, UnitPixel);
}

void CInventoryUI::Release()
{
}
