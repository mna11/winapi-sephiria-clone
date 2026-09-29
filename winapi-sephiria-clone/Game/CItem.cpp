#include "pch.h"
#include "CItem.h"

CItem::CItem()
	: m_iLevel(0)
{
	ZeroMemory(&m_tItemInfo, sizeof(ITEM_INFO));
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
	switch (iID)
	{
	case 0:
		// 정보 초기화
		SetItemInfo(iID, 5, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 1:
		// 정보 초기화
		SetItemInfo(iID, 5, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 2:
		// 정보 초기화
		SetItemInfo(iID, 5, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 3:
		// 정보 초기화
		SetItemInfo(iID, 5, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 4:
		// 정보 초기화
		SetItemInfo(iID, 5, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 5:
		// 정보 초기화
		SetItemInfo(iID, 5, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	default:
		break;
	}
}

void CItem::UpdateData()
{
	switch (m_tItemInfo.iID)
	{
	case 0:
		switch (m_iLevel)
		{
		case 0:
			m_tStat.iAtk = 10;
			m_tItemInfo.strDescription += L"1렙";
			break;
		case 1:
			m_tStat.iAtk = 15;
			m_tItemInfo.strDescription += L"1렙";
			break;
		case 2:
			m_tStat.iAtk = 20;
			m_tItemInfo.strDescription += L"2렙";
			break;
		case 3:
			m_tStat.iAtk = 25;
			m_tItemInfo.strDescription += L"3렙";
			break;
		case 4:
			m_tStat.iAtk = 30;
			m_tItemInfo.strDescription += L"4렙";
			break;
		case 5:
			m_tStat.iAtk = 40;
			m_tItemInfo.strDescription += L"5렙";
			break;
		default:
			break;
		}
		break;
	case 1:
		switch (m_iLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	case 2:
		switch (m_iLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	case 3:
		switch (m_iLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	case 4:
		switch (m_iLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	case 5:
		switch (m_iLevel)
		{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		case 5:
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CItem::AddLevel(int iLevel)
{
	m_iLevel += iLevel;
	if (m_iLevel <= 0)
		m_iLevel = 0; 

	UpdateData();
}
