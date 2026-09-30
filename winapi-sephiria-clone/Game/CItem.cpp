#include "pch.h"
#include "CItem.h"

CItem::CItem()
	: m_iLevel(0), m_iID(0)
{
}

CItem::~CItem()
{
	Release();
}

void CItem::Initialize()
{
}

int CItem::Update()
{
	if (m_bDead)
		return DEAD;

	return NOEVENT;
}

void CItem::LateUpdate()
{
}

void CItem::Render(Graphics*)
{
}

void CItem::Release()
{
}

void CItem::InitializeData(int iID)
{
	m_iLevel = 0;
	m_iID = iID;

	const ITEM_INFO* pItemInfo = CItemData::GetInstance()->FindItemInfo(m_iID);
	m_tStat = pItemInfo->vecStat[m_iLevel];
}

void CItem::ChangeLevel(int iLevel)
{
	// 최대 레벨보다 클 수도 있지만, 스탯이 높아지지는 않고
	// 0보다 작아질 수 있지만 스탯 적용이 안됨
	m_iLevel = iLevel;

	m_pTarget->AddStat(m_tStat * -1);

	const ITEM_INFO* pItemInfo = CItemData::GetInstance()->FindItemInfo(m_iID);
	int iMaxLevel = pItemInfo->vecStat.size() - 1; // 3렙까지 (0 1 2 3)
	if (iLevel > iMaxLevel)
		m_tStat = pItemInfo->vecStat[iMaxLevel];
	else if (iLevel < 0)
		m_tStat = STAT{}; 
	else
		m_tStat = pItemInfo->vecStat[iLevel];

	m_pTarget->AddStat(m_tStat);
}