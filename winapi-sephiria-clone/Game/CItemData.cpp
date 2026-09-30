#include "pch.h"
#include "CItemData.h"

CItemData* CItemData::m_pInstance = nullptr;

CItemData::CItemData()
{
}

CItemData::~CItemData()
{
	Release();
}

void CItemData::Initialize()
{
	// 정밀
	// 0. 발리송
	EmplaceItemInfo(0, L"Balisong", L"Precision", L"발리송", L"정밀", L"물리 피해 +6/8/11/14", L"", L"", CreateStatVec(0), ITEM_CATEGORY::PRECISION);
	// 1. 열망의 부적
	EmplaceItemInfo(1, L"Charm_Of_Aspiration", L"Precision", L"열망의 부적", L"정밀", L"치명타 확률 +3/6/10/14/20% 증가", L"", L"", CreateStatVec(1), ITEM_CATEGORY::PRECISION);
	// 2. 뾰족한 방망이
	EmplaceItemInfo(2, L"Pointed_Club", L"Precision", L"뾰족한 방망이", L"정밀", L"치명타 피해 +20/40/60%", L"", L"", CreateStatVec(2), ITEM_CATEGORY::PRECISION);
	
	// 그림자
	// 3. 부서진 사파이어
	EmplaceItemInfo(3, L"Broken_Sapphire", L"Shadow", L"부서진 사파이어", L"그림자", L"회피 +2/3/4/5", L"이동속도 +5/7/9/12%", L"", CreateStatVec(3), ITEM_CATEGORY::SHADOW);
	// 4. 환락의 망토
	EmplaceItemInfo(4, L"Cloak_Of_Verdant_Spirit", L"Shadow", L"환락의 망토", L"그림자", L"대시 횟수 +0/1/1", L"회피 +4/6/8", L"치명타 확률 +1/2/4%", CreateStatVec(4), ITEM_CATEGORY::SHADOW);
	// 5. 모형 부리
	EmplaceItemInfo(5, L"Model_Beak", L"Shadow", L"모형 부리", L"그림자", L"물리 피해 +1/2/3/5/8", L"회피 +2/3/4/5/6", L"", CreateStatVec(5), ITEM_CATEGORY::SHADOW);
	
	// 수호
	// 6. 천 갑옷
	EmplaceItemInfo(6, L"Cloth_Armor", L"Guardian", L"천 갑옷", L"수호", L"방어력 +3/6/9/12", L"", L"", CreateStatVec(6), ITEM_CATEGORY::GUARDIAN);
	// 7. 방패 귀고리
	EmplaceItemInfo(7, L"Shield_Earrings", L"Guardian", L"방패 귀고리", L"수호", L"최대 HP +5/10/15/20", L"회피 -5/4/3/2", L"", CreateStatVec(7), ITEM_CATEGORY::GUARDIAN);
	// 8. 냄비 뚜껑
	EmplaceItemInfo(8, L"Pan_Lid", L"Guardian", L"냄비 뚜껑", L"수호", L"MP 재생 +2/4/6/8/11/15", L"회피 +1/2/3/4/5/6", L"", CreateStatVec(8), ITEM_CATEGORY::GUARDIAN);
}

void CItemData::Release()
{
	m_mapItemInfo.clear();
}

void CItemData::EmplaceItemInfo(int iID, wstring strImg, wstring strCategoryImg, wstring strName, wstring strCategoryName, wstring strDescription1, wstring strDescription2, wstring strDescription3, vector<tagStat> vecStat, ITEM_CATEGORY eCategory)
{
	ITEM_INFO tItemInfo;
	tItemInfo.iID = iID;
	tItemInfo.strImg = move(strImg);
	tItemInfo.strCategoryImg = move(strCategoryImg);

	tItemInfo.strName = move(strName);
	tItemInfo.strCategoryName = move(strCategoryName);

	tItemInfo.strDescription1 = move(strDescription1);
	tItemInfo.strDescription2 = move(strDescription2);
	tItemInfo.strDescription3 = move(strDescription3);
	
	tItemInfo.vecStat = move(vecStat);

	tItemInfo.eCategory = eCategory;

	m_mapItemInfo.emplace(iID, move(tItemInfo));
}

vector<STAT> CItemData::CreateStatVec(int iID)
{
	vector<STAT> vecStat{};

	switch (iID)
	{
	case 0: // 발리송
		vecStat.resize(4);
		vecStat[0].iPhysicalAtk = 6;
		vecStat[1].iPhysicalAtk = 8;
		vecStat[2].iPhysicalAtk = 11;
		vecStat[3].iPhysicalAtk = 14;
		break;
	case 1: // 열망의 부적
		vecStat.resize(5);
		vecStat[0].fCriticalChange = 3.f;
		vecStat[1].fCriticalChange = 6.f;
		vecStat[2].fCriticalChange = 10.f;
		vecStat[3].fCriticalChange = 14.f;
		vecStat[4].fCriticalChange = 20.f;
		break;
	case 2: // 뾰족한 방망이
		vecStat.resize(3);
		vecStat[0].fCriticalDamage = 20.f;
		vecStat[1].fCriticalDamage = 40.f;
		vecStat[2].fCriticalDamage = 60.f;
		break;
	case 3: // 부서진 사파이어
		vecStat.resize(4);
		vecStat[0].iEvasion = 2;
		vecStat[0].fMoveSpeed = 5.f;

		vecStat[1].iEvasion = 3;
		vecStat[1].fMoveSpeed = 7.f;

		vecStat[2].iEvasion = 4;
		vecStat[2].fMoveSpeed = 9.f;

		vecStat[3].iEvasion = 5;
		vecStat[3].fMoveSpeed = 12.f;
		break;
	case 4: // 환락의 망토
		vecStat.resize(3);
		vecStat[0].iMaxDash = 0;
		vecStat[0].iEvasion = 4;
		vecStat[0].fCriticalChange = 1.f;

		vecStat[1].iMaxDash = 1;
		vecStat[1].iEvasion = 6;
		vecStat[1].fCriticalChange = 2.f;

		vecStat[2].iMaxDash = 1;
		vecStat[2].iEvasion = 8;
		vecStat[2].fCriticalChange = 4.f;
		break;
	case 5: // 모형 부리
		vecStat.resize(5);
		vecStat[0].iPhysicalAtk = 1;
		vecStat[0].iEvasion = 2;

		vecStat[1].iPhysicalAtk = 2;
		vecStat[1].iEvasion = 3;

		vecStat[2].iPhysicalAtk = 3;
		vecStat[2].iEvasion = 4;

		vecStat[3].iPhysicalAtk = 5;
		vecStat[3].iEvasion = 5;

		vecStat[4].iPhysicalAtk = 8;
		vecStat[4].iEvasion = 6;
		break;
	case 6: // 천 갑옷
		vecStat.resize(4);
		vecStat[0].iDefense = 3;
		vecStat[1].iDefense = 6;
		vecStat[2].iDefense = 9;
		vecStat[3].iDefense = 12;
		break;
	case 7: // 방패 귀고리
		vecStat.resize(4);
		vecStat[0].iHp = 5;
		vecStat[0].iMaxHp = 5;
		vecStat[0].iEvasion = -5;

		vecStat[1].iHp = 10;
		vecStat[1].iMaxHp = 10;
		vecStat[1].iEvasion = -4;

		vecStat[2].iHp = 15;
		vecStat[2].iMaxHp = 15;
		vecStat[2].iEvasion = -3;

		vecStat[3].iHp = 20;
		vecStat[3].iMaxHp = 20;
		vecStat[3].iEvasion = -2;
		break;
	case 8: // 냄비 뚜껑
		vecStat.resize(6);
		vecStat[0].iMpRegeneration = 2;
		vecStat[0].iEvasion = 1;

		vecStat[1].iMpRegeneration = 4;
		vecStat[1].iEvasion = 2;

		vecStat[2].iMpRegeneration = 6;
		vecStat[2].iEvasion = 3;

		vecStat[3].iMpRegeneration = 8;
		vecStat[3].iEvasion = 4;

		vecStat[4].iMpRegeneration = 11;
		vecStat[4].iEvasion = 5;

		vecStat[5].iMpRegeneration = 15;
		vecStat[5].iEvasion = 6;
		break;
	default:
		break;
	}

	return vecStat;
}

const ITEM_INFO* CItemData::FindItemInfo(int iID) const
{
	auto	iter = m_mapItemInfo.find(iID); 

	if (iter == m_mapItemInfo.end())
		return nullptr;

	return &iter->second;
}
