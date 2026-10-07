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


void CStoneTabletData::EmplaceStoneTabletInfo(int iID, wstring strImg, wstring strName, array<vector<pair<int, int>>, 4> arrRelativePos, vector<int> vecApplyLevel, int iLeaf)
{
	STONE_TABLET_INFO tStoneTabletInfo;
	tStoneTabletInfo.iID = iID;
	tStoneTabletInfo.strImg = move(strImg);
	tStoneTabletInfo.strName = move(strName);
	tStoneTabletInfo.arrRelativePos = move(arrRelativePos);
	tStoneTabletInfo.vecApplyLevel = move(vecApplyLevel);
	tStoneTabletInfo.iLeaf = iLeaf;

	m_mapStoneTabletInfo.emplace(iID, move(tStoneTabletInfo));
}

array<vector<pair<int, int>>,4> CStoneTabletData::CreateRelativePos(int iID)
{
	array<vector<pair<int, int>>, 4> arrRelativePos;

	switch (iID)
	{
	case 0: // 도래
		for_each(arrRelativePos.begin(), arrRelativePos.end(), [](auto& vecRelativePos)
			{
				vecRelativePos.reserve(4);
			});

		arrRelativePos[0].emplace_back(0, -2);
		arrRelativePos[0].emplace_back(0, -1);
		arrRelativePos[0].emplace_back(0, 1);
		arrRelativePos[0].emplace_back(0, 2);
		break;
	case 1: // 근사
		for_each(arrRelativePos.begin(), arrRelativePos.end(), [](auto& vecRelativePos)
			{
				vecRelativePos.reserve(2);
			});

		arrRelativePos[0].emplace_back(0, -1);
		arrRelativePos[0].emplace_back(1, 0);
		break;
	case 2: // 고동
		for_each(arrRelativePos.begin(), arrRelativePos.end(), [](auto& vecRelativePos)
			{
				vecRelativePos.reserve(1);
			});
		arrRelativePos[0].emplace_back(0, -2);
		break;
	//case 3: // 기반
	//	for_each(arrRelativePos.begin(), arrRelativePos.end(), [](auto& vecRelativePos)
	//		{
	//			vecRelativePos.reserve(10);
	//		});
	//	arrRelativePos[0].emplace_back(-5, 0);
	//	arrRelativePos[0].emplace_back(-4, 0);
	//	arrRelativePos[0].emplace_back(-3, 0);
	//	arrRelativePos[0].emplace_back(-2, 0);
	//	arrRelativePos[0].emplace_back(-1, 0);
	//	arrRelativePos[0].emplace_back(1, 0);
	//	arrRelativePos[0].emplace_back(2, 0);
	//	arrRelativePos[0].emplace_back(3, 0);
	//	arrRelativePos[0].emplace_back(4, 0);
	//	arrRelativePos[0].emplace_back(5, 0);
	//	break;
	case 3: // 입구
		for_each(arrRelativePos.begin(), arrRelativePos.end(), [](auto& vecRelativePos)
			{
				vecRelativePos.reserve(3);
			});
		arrRelativePos[0].emplace_back(-1, -1);
		arrRelativePos[0].emplace_back(0, -1);
		arrRelativePos[0].emplace_back(1, -1);
		break;
	case 4: // 미래
		for_each(arrRelativePos.begin(), arrRelativePos.end(), [](auto& vecRelativePos)
			{
				vecRelativePos.reserve(4);
			});
		arrRelativePos[0].emplace_back(-1, -1);
		arrRelativePos[0].emplace_back(0, -1);
		arrRelativePos[0].emplace_back(1, -1);
		arrRelativePos[0].emplace_back(-1, 0);
		break;
	default:
		break;
	}

	for (int i = 1; i < 4; ++i)
	{
		for (int j = 0; j < arrRelativePos[0].size(); ++j)
		{
			VEC vCord = { static_cast<float>(arrRelativePos[i - 1][j].first), static_cast<float>(arrRelativePos[i - 1][j].second) };
			VEC vRotateCord = VEC(0, -1) * vCord.fX + VEC(1, 0) * vCord.fY;
			arrRelativePos[i].emplace_back(static_cast<int>(vRotateCord.fX), static_cast<int>(vRotateCord.fY));
		}
	}

	return arrRelativePos;
}

vector<int> CStoneTabletData::CreatApplyLevel(int iID)
{
	vector<int> vecApplyLevel;

	switch (iID)
	{
	case 0: // 도래
		vecApplyLevel.reserve(4);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(-1);
		vecApplyLevel.emplace_back(-1);
		break;
	case 1: // 근사
		vecApplyLevel.reserve(2);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(1);
		break;
	case 2: // 고동
		vecApplyLevel.reserve(1);
		vecApplyLevel.emplace_back(2);
		break;
	//case 3: // 기반
	//	vecApplyLevel.reserve(10);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	vecApplyLevel.emplace_back(1);
	//	break;
	case 3: // 입구
		vecApplyLevel.reserve(3);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(2);
		vecApplyLevel.emplace_back(1);
		break;
	case 4: // 미래
		vecApplyLevel.reserve(4);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(1);
		vecApplyLevel.emplace_back(1);
		break;
	default:
		break;
	}

	return vecApplyLevel;
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
	//CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Foundation.png", L"Foundation");
	// 3. 헌정 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Entrance.png", L"Entrance");
	// 4. 미래
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/StoneTablet/Future.png", L"Future");
}

void CStoneTabletData::InitializeStoneTabletInfo()
{
	EmplaceStoneTabletInfo(0, L"Advent", L"도래", CreateRelativePos(0), CreatApplyLevel(0), 100);
	EmplaceStoneTabletInfo(1, L"Approximation", L"근사", CreateRelativePos(1), CreatApplyLevel(1), 200);
	EmplaceStoneTabletInfo(2, L"Pulsation", L"고동", CreateRelativePos(2), CreatApplyLevel(2), 300);
	//EmplaceStoneTabletInfo(3, L"Foundation", L"기반", CreateRelativePos(3), CreatApplyLevel(3), 400);
	EmplaceStoneTabletInfo(3, L"Entrance", L"입구", CreateRelativePos(3), CreatApplyLevel(3), 400);
	EmplaceStoneTabletInfo(4, L"Future", L"미래", CreateRelativePos(4), CreatApplyLevel(4), 500);
}