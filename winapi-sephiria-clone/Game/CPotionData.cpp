#include "pch.h"
#include "CPotionData.h"

#include "CImgMgr.h"

CPotionData* CPotionData::m_pInstance = nullptr;

CPotionData::CPotionData()
{
}

CPotionData::~CPotionData()
{
	Release();
}

void CPotionData::Initialize()
{
	InitializePotionInfo();
	InitializeImg();
}

void CPotionData::Release()
{
	m_mapPotionInfo.clear();
}

void CPotionData::EmplaceArtifactInfo(int iID, wstring strImg, wstring strName, vector<wstring> vecStrDescription, STAT tStat, int iMaxHave, int iLeaf)
{
	POTION_INFO tPotionInfo;
	tPotionInfo.iID = iID;
	tPotionInfo.strImg = move(strImg);

	tPotionInfo.strName = move(strName);

	tPotionInfo.vecStrDescription = move(vecStrDescription);
	tPotionInfo.tStat = move(tStat);
	tPotionInfo.iMaxHave = iMaxHave;
	tPotionInfo.iLeaf = iLeaf;

	m_mapPotionInfo.emplace(iID, move(tPotionInfo));
}

STAT CPotionData::CreateStat(int iID)
{
	STAT tStat{};

	switch (iID)
	{
	case 0: // 멸치
		tStat.iPhysicalAtk = 5;
		break;

	case 1: // 참치
		tStat.fCriticalChange = 0.05f;
		break;

	case 2: // 배스
		tStat.fCriticalDamage = 0.1f;
		break;

	case 3: // HP 포션
		tStat.iHp = 20.f;
		break;

	case 4: // MP 포션
		tStat.iMp = 20.f;
		break;
	}

	return tStat;
}

vector<wstring> CPotionData::CreateStrDescriptionVec(int iID)
{
	vector<wstring> vecStrDescription;

	switch (iID)
	{
	case 0: // 멸치
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"물리 공격력 +5");
		vecStrDescription.push_back(L"소지 상한 : 3개");
		break;

	case 1: // 참치
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"치명타 확률 +5%");
		vecStrDescription.push_back(L"소지 상한 : 3개");
		break;

	case 2: // 배스
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"치명타 피해 +10%");
		vecStrDescription.push_back(L"소지 상한 : 3개");
		break;

	case 3: // HP 물약
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"HP +20% 회복");
		vecStrDescription.push_back(L"소지 상한 : 5개");
		break;

	case 4: // MP 물약
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"MP +20% 회복");
		vecStrDescription.push_back(L"소지 상한 : 5개");
		break;
	}

	return vecStrDescription;
}

const POTION_INFO* CPotionData::FindPotionInfo(int iID) const
{
	auto	iter = m_mapPotionInfo.find(iID);

	if (iter == m_mapPotionInfo.end())
		return nullptr;

	return &iter->second;
}

ITEM_INFO CPotionData::FindItemInfo(int iID)
{
	const POTION_INFO* pArtifactInfo = FindPotionInfo(iID);

	if (nullptr == pArtifactInfo)
		return ITEM_INFO{ -1, ITEM_TYPE::END, {}, {}, -1 };

	return ITEM_INFO{ iID, ITEM_TYPE::POTION, pArtifactInfo->strName, pArtifactInfo->strImg, pArtifactInfo->iLeaf };
}


void CPotionData::InitializeImg()
{
	// 아이템 아이콘
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Potion/Fish/Anchovy.png", L"Anchovy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Potion/Fish/Tuna.png", L"Tuna");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Potion/Fish/SeaBass.png", L"SeaBass");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Potion/RedPotion.png", L"RedPotion");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Potion/BluePotion.png", L"BluePotion");
}

void CPotionData::InitializePotionInfo()
{
	// 0. 멸치
	EmplaceArtifactInfo(0, L"Anchovy", L"멸치", CreateStrDescriptionVec(0), CreateStat(0), 3, 200);
	// 1. 참치
	EmplaceArtifactInfo(1, L"Tuna", L"참치", CreateStrDescriptionVec(1), CreateStat(1), 3, 300);
	// 2. 배스
	EmplaceArtifactInfo(2, L"SeaBass", L"배스", CreateStrDescriptionVec(2), CreateStat(2), 3, 400);
	// 3. HP 물약
	EmplaceArtifactInfo(3, L"RedPotion", L"재생의 물약", CreateStrDescriptionVec(3), CreateStat(3), 5, 100);
	// 4. MP 물약
	EmplaceArtifactInfo(4, L"BluePotion", L"집중의 포션", CreateStrDescriptionVec(4), CreateStat(4), 5, 100);
}