#include "pch.h"
#include "CItem.h"

CItem::CItem()
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

void CItem::UpdateData(int iID)
{
	switch (iID)
	{
	case 0:
		// 정보 초기화
		SetItemInfo(iID, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 1:
		// 정보 초기화
		SetItemInfo(iID, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 2:
		// 정보 초기화
		SetItemInfo(iID, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 3:
		// 정보 초기화
		SetItemInfo(iID, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 4:
		// 정보 초기화
		SetItemInfo(iID, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	case 5:
		// 정보 초기화
		SetItemInfo(iID, L"Balisong", L"발리송", L"발리송 아이템 설명입니다.", ITEM_CATEGORY::PRECISION);
		// 스탯 초기화
		m_tStat.iAtk = 10;
		break;
	default:
		break;
	}
}
