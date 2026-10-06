#include "pch.h"
#include "CItem.h"

#include "CArtifactData.h"
#include "CStoneTabletData.h"

CItem::CItem()
	: m_iLevel(0), m_iID(0), m_eItemType(ITEM_TYPE::END)
{
}

CItem::~CItem()
{
}

void CItem::InitializeData(int iID, ITEM_TYPE eItemType)
{
	m_iLevel = 0;
	m_iID = iID;
	m_eItemType = eItemType;

	switch (eItemType)
	{
	case ITEM_TYPE::ARTIFACT:
	{
		const ARTIFACT_INFO* pArtifactInfo = CArtifactData::GetInstance()->FindArtifactInfo(m_iID);
		m_tStat = pArtifactInfo->vecStat[m_iLevel];
		break;
	}
	case ITEM_TYPE::STONE_TABLET:
	{
		// 뭐 할 필요가 있나? 일단 패스
		break;
	}
	default:
		break;
	}
}