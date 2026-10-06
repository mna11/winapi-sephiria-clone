#include "pch.h"
#include "CItemSelectSlotUI.h"

#include "CTimeMgr.h"
#include "CImgMgr.h"
#include "CArtifactData.h"
#include "CStoneTabletData.h"

CItemSelectSlotUI::CItemSelectSlotUI()
	:m_iItemID(-1), m_bSpread(false), m_fDistance(0.f), m_bCol(false), m_eItemType(ITEM_TYPE::END)
{
}

CItemSelectSlotUI::~CItemSelectSlotUI()
{
	Release();
}

void CItemSelectSlotUI::Initialize()
{
	m_fUIScale = PIXEL_SCALE * 0.5f;
	m_tInfo = { WINCX >> 1, WINCY >> 1, 46.f * m_fUIScale, 47.f * m_fUIScale };
	m_iItemID = -1;
	m_bSpread = false;
	m_fDistance = 0.f;
	m_bCol = false;
	m_bView = false;
	m_fSpeed = 300.f;
}

int CItemSelectSlotUI::Update()
{
	if (!m_bView)
		return NOEVENT;

	Move();

	__super::UpdateRect();
	return NOEVENT;
}

void CItemSelectSlotUI::LateUpdate()
{
	if (!m_bView)
		return;

}

void CItemSelectSlotUI::Render(Graphics* pGraphics)
{
	if (!m_bView)
		return;

	Image* pImg(nullptr);
	VEC vCellSize{ 46.f, 47.f };
	VEC vImgSize = vCellSize * m_fUIScale;

	pImg = CImgMgr::GetInstance()->FindImg(L"Sephirite_Slot");
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

	if (-1 == m_iItemID || ITEM_TYPE::END == m_eItemType)
		return;

	ITEM_INFO tItemInfo{};
	switch (m_eItemType)
	{
	case ITEM_TYPE::ARTIFACT:
		tItemInfo = CArtifactData::GetInstance()->FindItemInfo(m_iItemID);
		break;
	case ITEM_TYPE::STONE_TABLET:
		tItemInfo = CStoneTabletData::GetInstance()->FindItemInfo(m_iItemID);
		break;
	default:
		break;
	}

	if (-1 == tItemInfo.iID)
		return; 

	// 아이템 그리기
	Image* pItemImg = CImgMgr::GetInstance()->FindImg(tItemInfo.strImg.c_str());
	vCellSize = { 32.f, 32.f };
	vImgSize = VEC{ vCellSize.fX + 14.f, vCellSize.fY + 15.f } * m_fUIScale;
	vImgSize = vCellSize * m_fUIScale;
	rcDest = { m_tInfo.vPoint.fX - vImgSize.fX * 0.5f,
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

void CItemSelectSlotUI::Release()
{
}

void CItemSelectSlotUI::Move()
{
	if (nullptr == m_pTarget)
		return;

	// CItemSelectUI의 이동량만큼 더해줌 -> CItemSelectUI를 따라감
	m_tInfo.vPoint += m_pTarget->GetInfo().vPoint - m_pTarget->GetPrePoint();

	// 퍼지기
	if (!m_bSpread)
		return;

	// 방향 벡터 방향으로 이동
	m_tInfo.vPoint += m_vDir * m_fSpeed * DT;
	float fCurDistance = (m_tInfo.vPoint - m_pTarget->GetInfo().vPoint).Norm();

	if (fCurDistance >= m_fDistance)
	{
		m_tInfo.vPoint = m_pTarget->GetInfo().vPoint + m_vDir * m_fDistance;
		m_bSpread = false;
	}
}