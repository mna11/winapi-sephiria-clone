#include "pch.h"
#include "CStoneTablet.h"

#include "CStoneTabletData.h"

CStoneTablet::CStoneTablet()
{
}

CStoneTablet::~CStoneTablet()
{
	Release();
}

void CStoneTablet::Initialize()
{
}

int CStoneTablet::Update()
{
	if (m_bDead)
		return DEAD;

	return NOEVENT;
}

void CStoneTablet::LateUpdate()
{
}

void CStoneTablet::Render(Graphics*)
{
}

void CStoneTablet::Release()
{
}

void CStoneTablet::InitializeData(int iID, ITEM_TYPE eType)
{
	m_iLevel = 0;
	m_iID = iID;
	m_eItemType = eType;

	const STONE_TABLET_INFO* pStoneTabletInfo = CStoneTabletData::GetInstance()->FindStoneTabletInfo(m_iID);
	//m_vecRelativePos = pStoneTabletInfo->vecRelativePos;
}
