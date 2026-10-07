#include "pch.h"
#include "CInventorySlotUI.h"
#include "CItem.h"

#include "CArtifact.h"

#include "CImgMgr.h"
#include "CFontMgr.h"
#include "CArtifactData.h"
#include "CStoneTabletData.h"

CInventorySlotUI::CInventorySlotUI()
	: m_pItem(nullptr), m_bCol(false), m_iSlotLevel(0)
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
	m_iRenderLayer = 2;
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

	if (nullptr == m_pItem)
		m_pFrameKey = L"Inventory_Slot_Blank";
	else if (m_pItem->GetItemType() == ITEM_TYPE::ARTIFACT)
		m_pFrameKey = L"Inventory_Slot_Artifact";
	else if (m_pItem->GetItemType() == ITEM_TYPE::STONE_TABLET)
		m_pFrameKey = L"Inventory_Slot_StoneTablet";
	else
		m_pFrameKey = L"Inventory_Slot_Blank";

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

	if (nullptr != m_pItem)
	{
		// 아이템 그리기
		Matrix matRot;
		Image* pItemImg = CImgMgr::GetInstance()->FindImg(m_pItem->GetItemInfo().strImg.c_str());

		float fAngle = m_pItem->GetAngle() * 180.f / PI * -1;
		matRot.RotateAt(fAngle, { m_tInfo.vPoint.fX, m_tInfo.vPoint.fY });
		pGraphics->SetTransform(&matRot);

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

		pGraphics->ResetTransform();

		// 레벨 그리기
		if (m_pItem->GetItemType() == ITEM_TYPE::ARTIFACT)
		{
			// 아이템 레벨 그리기
			int iMaxLevel = static_cast<CArtifact*>(m_pItem)->GetArtifactInfo()->vecStat.size();
			int iLevel = m_pItem->GetLevel();
			wstring strLevel = to_wstring(iLevel) + L"/" + to_wstring(iMaxLevel);
			VEC vOffset{ 5.f * m_fUIScale, 5.f * m_fUIScale };
			RectF DestRect{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vOffset.fX,
							m_tInfo.vPoint.fY - vImgSize.fX * 0.5f + vOffset.fY,
							50.f, 50.f };
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

			return;
		}
	}

	if (0 == m_iSlotLevel)
		return;
		
	wstring strLevel{};
	Color   levelColor{};

	// 음수는 알아서 - 붙음
	if (m_iSlotLevel > 0)
	{
		strLevel += L"+";
		levelColor = { 255, 255, 255, 255 };
	}
	else
	{
		levelColor = { 255, 255, 0, 0 };
	}

	strLevel += to_wstring(m_iSlotLevel);
	VEC vOffset{ 5.f * m_fUIScale, 5.f * m_fUIScale };
	RectF DestRect{ m_tInfo.vPoint.fX - vImgSize.fX * 0.5f + vOffset.fX,
						m_tInfo.vPoint.fY - vImgSize.fX * 0.5f + vOffset.fY,
						50.f, 50.f };

	CFontMgr::GetInstance()->DrawString(pGraphics, strLevel, FONT_TYPE::PIXEL_SMALL, DestRect, levelColor, 12.f, StringAlignmentNear, StringAlignmentNear);
}

void CInventorySlotUI::Release()
{
}
