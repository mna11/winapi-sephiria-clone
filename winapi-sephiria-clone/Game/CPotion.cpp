#include "pch.h"
#include "CPotion.h"

CPotion::CPotion()
	: m_iHave(0)
{
}

CPotion::~CPotion()
{
	Release();
}

void CPotion::Initialize()
{
}

int CPotion::Update()
{
	if (m_bDead)
		return DEAD;

	return NOEVENT;
}

void CPotion::LateUpdate()
{
}

void CPotion::Render(Graphics*)
{
}

void CPotion::Release()
{
}

void CPotion::SetHave(int iAmount)
{
	const POTION_INFO* pPotionInfo = GetPotionInfo();
	if (nullptr == pPotionInfo)
		return;

	m_iHave = clamp(iAmount, 0, pPotionInfo->iMaxHave);
}

void CPotion::AddHave(int iAmount)
{
	const POTION_INFO* pPotionInfo = GetPotionInfo();
	if (nullptr == pPotionInfo)
		return;

	m_iHave = clamp(m_iHave + iAmount, 0, pPotionInfo->iMaxHave);
}

void CPotion::InitializeData(int iID, ITEM_TYPE eType)
{
	m_iLevel = 0;
	m_iID = iID;
	m_eItemType = eType;

	const POTION_INFO* pPotionInfo = CPotionData::GetInstance()->FindPotionInfo(m_iID);
}
