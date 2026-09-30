#include "pch.h"
#include "CInventorySlotUI.h"
#include "CItem.h"

#include "CImgMgr.h"
#include "CFontMgr.h"

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
	m_tInfo = { 0.f, 0.f, 34.f * m_fUIScale, 34.f * m_fUIScale };

	// 렌더 정보 초기화 
	m_eRender = RENDERID::UI;
	m_iRenderLayer = 4;      // UI 중에 최강
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
	Image* pImg(nullptr);
	VEC vCellSize{ 32.f, 32.f };
	VEC vImgSize = vCellSize * m_fUIScale;

	m_pFrameKey = (nullptr == m_pItem ? L"Inventory_Slot_Blank" : L"Inventory_Slot_Item");
	pImg = CImgMgr::GetInstance()->FindImg(m_pFrameKey);
	if (nullptr == pImg)
		return;

	RectF rcDest{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
					m_tInfo.vPoint.fY - vImgSize.fY * 0.5f,
					vImgSize.fX,
					vImgSize.fY };

	pGraphics->DrawImage(
		pImg, rcDest,
		(m_bCol ? vCellSize.fX : 0),
		0,
		vCellSize.fX,
		vCellSize.fY,
		UnitPixel
	);

	if (nullptr == m_pItem)
		return;

	// 아이템 그리기
	Image* pItemImg = CImgMgr::GetInstance()->FindImg(m_pItem->GetItemInfo()->strImg.c_str());
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

	// 아이템 레벨 그리기
	int iMaxLevel = m_pItem->GetItemInfo()->vecStat.size();
	int iLevel = m_pItem->GetLevel();
	wstring strLevel = to_wstring(iLevel) + L"/" + to_wstring(iMaxLevel);
	VEC vOffset{ 5.f * m_fUIScale, 5.f * m_fUIScale };
	RectF DestRect{	m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vOffset.fX, 
					m_tInfo.vPoint.fY - vImgSize.fX * 0.5f + vOffset.fY, 
					50.f, 50.f};
	Color levelColor{};
	if (iLevel < 0)
		levelColor = { 255, 255, 0, 0 };
	else if (iLevel == iMaxLevel)
		levelColor = { 255, 0, 255, 0 };
	else if (iLevel > iMaxLevel)
		levelColor = { 255, 255, 255, 0 };
	else
		levelColor = { 255, 255, 255, 255 };

	CFontMgr::GetInstance()->DrawString(pGraphics, strLevel, FONT_TYPE::PIXEL_SMALL, DestRect, levelColor, 12.f, StringAlignmentNear, StringAlignmentNear);
}

void CInventorySlotUI::Release()
{
}
