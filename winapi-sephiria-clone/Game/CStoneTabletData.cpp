#include "pch.h"
#include "CStoneTabletData.h"

#include "CImgMgr.h"

CStoneTabletData* CStoneTabletData::m_pInstance = nullptr;

CStoneTabletData::CStoneTabletData()
{
}

CStoneTabletData::~CStoneTabletData()
{
	Release();
}

void CStoneTabletData::Initialize()
{
	InitializeStoneTabletInfo();
	InitializeImg();
}

void CStoneTabletData::Release()
{
	m_mapStoneTabletInfo.clear();
}

void CStoneTabletData::EmplaceStoneTabletInfo(int iID, wstring strImg, wstring strName, array<int, INVEN_SIZE> arrApplyLevel, int iLeaf)
{
	STONE_TABLET_INFO tStoneTabletInfo;
	tStoneTabletInfo.iID = iID;
	tStoneTabletInfo.strImg = move(strImg);
	tStoneTabletInfo.strName = move(strName);
	tStoneTabletInfo.arrApplyLevel = move(arrApplyLevel);
	tStoneTabletInfo.iLeaf = iLeaf;

	m_mapStoneTabletInfo.emplace(iID, move(tStoneTabletInfo));
}

array<int, INVEN_SIZE> CStoneTabletData::CreateApplyLevel(int iID)
{
	array<int, INVEN_SIZE> arrApplyLevel;

	return array<int, INVEN_SIZE>();
}

const STONE_TABLET_INFO* CStoneTabletData::FindStoneTabletInfo(int iID) const
{
	auto	iter = m_mapStoneTabletInfo.find(iID); 

	if (iter == m_mapStoneTabletInfo.end())
		return nullptr;

	return &iter->second;
}

ITEM_INFO CStoneTabletData::FindItemInfo(int iID)
{
	const STONE_TABLET_INFO* pStoneTabletInfo = FindStoneTabletInfo(iID);

	if (nullptr == pStoneTabletInfo)
		return ITEM_INFO{ -1, ITEM_TYPE::END, {}, {}, - 1 };

	return ITEM_INFO{ iID, ITEM_TYPE::STONE_TABLET, pStoneTabletInfo->strName, pStoneTabletInfo->strImg, pStoneTabletInfo->iLeaf };
}

void CStoneTabletData::InitializeImg()
{
	// 0. 도래
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Advent.png", L"Advent");
	// 1. 근사
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Approximation.png", L"Approximation");
	// 2. 고동
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Pulsation.png", L"Pulsation");
	// 3. 기반
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Foundation.png", L"Foundation");
	// 4. 미래
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Future.png", L"Future");
}

void CStoneTabletData::InitializeStoneTabletInfo()
{
	EmplaceStoneTabletInfo(0, L"Advent", L"도래", CreateApplyLevel(0), 100);
	EmplaceStoneTabletInfo(1, L"Approximation", L"근사", CreateApplyLevel(1), 200);
	EmplaceStoneTabletInfo(2, L"Pulsation", L"고동", CreateApplyLevel(2), 300);
	EmplaceStoneTabletInfo(3, L"Foundation", L"기반", CreateApplyLevel(3), 400);
	EmplaceStoneTabletInfo(4, L"Future", L"미래", CreateApplyLevel(4), 500);
}