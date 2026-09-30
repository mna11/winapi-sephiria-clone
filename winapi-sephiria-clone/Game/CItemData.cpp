#include "pch.h"
#include "CItemData.h"

#include "CImgMgr.h"

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
	InitializeItemInfo();
	InitializeImg();
}

void CItemData::Release()
{
	m_mapItemInfo.clear();
}

void CItemData::EmplaceItemInfo(int iID, wstring strImg, wstring strCategoryImg, wstring strName, wstring strCategoryName, vector<wstring> vecStrDescription, vector<tagStat> vecStat, ITEM_CATEGORY eCategory)
{
	ITEM_INFO tItemInfo;
	tItemInfo.iID = iID;
	tItemInfo.strImg = move(strImg);
	tItemInfo.strCategoryImg = move(strCategoryImg);

	tItemInfo.strName = move(strName);
	tItemInfo.strCategoryName = move(strCategoryName);

	tItemInfo.vecStrDescription = move(vecStrDescription);
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

vector<wstring> CItemData::CreateStrDescriptionVec(int iID)
{
	vector<wstring> vecStrDescription{};

	switch (iID)
	{
	case 0: // 발리송
		vecStrDescription.reserve(1);
		vecStrDescription.push_back(L"물리 피해 +6/8/11/14");
		break;
	case 1: // 열망의 부적
		vecStrDescription.reserve(1);
		vecStrDescription.push_back(L"치명타 확률 +3/6/10/14/20%");
		break;
	case 2: // 뾰족한 방망이
		vecStrDescription.reserve(1);
		vecStrDescription.push_back(L"치명타 피해 +20/40/60%");
		break;
	case 3: // 부서진 사파이어
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"회피 +2/3/4/5");
		vecStrDescription.push_back(L"이동속도 +5/7/9/12%");
		break;
	case 4: // 환락의 망토
		vecStrDescription.reserve(3);
		vecStrDescription.push_back(L"대시 횟수 +0/1/1");
		vecStrDescription.push_back(L"회피 +4/6/8");
		vecStrDescription.push_back(L"치명타 확률 +1/2/4%");
		break;
	case 5: // 모형 부리
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"물리 피해 +1/2/3/5/8");
		vecStrDescription.push_back(L"회피 +2/3/4/5/6");
		break;
	case 6: // 천 갑옷
		vecStrDescription.reserve(1);
		vecStrDescription.push_back(L"방어력 +3/6/9/12");
		break;
	case 7: // 방패 귀고리
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"최대 HP +5/10/15/20");
		vecStrDescription.push_back(L"회피 -5/4/3/2");
		break;
	case 8: // 냄비 뚜껑
		vecStrDescription.reserve(2);
		vecStrDescription.push_back(L"MP 재생 +2/4/6/8/11/15");
		vecStrDescription.push_back(L"회피 +1/2/3/4/5/6");
		break;
	default:
		break;
	}

	return vecStrDescription;
}

const ITEM_INFO* CItemData::FindItemInfo(int iID) const
{
	auto	iter = m_mapItemInfo.find(iID); 

	if (iter == m_mapItemInfo.end())
		return nullptr;

	return &iter->second;
}


void CItemData::InitializeImg()
{
	// 시너지 아이콘
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Academy.png", L"Academy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Alchemy.png", L"Alchemy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Companion.png", L"Companion");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Curse.png", L"Curse");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Ember.png", L"Ember");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/FrostRelic.png", L"FrostRelic");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Glacier.png", L"Glacier");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Guardian.png", L"Guardian");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Lake.png", L"Lake");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Magitech.png", L"Magitech");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Mystic.png", L"Mystic");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Negotiation.png", L"Negotiation");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Planet.png", L"Planet");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Precision.png", L"Precision");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Shadow.png", L"Shadow");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/SolarBlade.png", L"SolarBlade");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/StormCloud.png", L"StormCloud");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/Sturdy.png", L"Sturdy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Category/WindSong.png", L"WindSong");

	// 아이템 아이콘
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Balisong.png", L"Balisong");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/CharmOfAspiration.png", L"Charm_Of_Aspiration");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/PointedClub.png", L"Pointed_Club");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/BrokenSapphire.png", L"Broken_Sapphire");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/CloakOfVerdantSpirit.png", L"Cloak_Of_Verdant_Spirit");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/ModelBeak.png", L"Model_Beak");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/ClothArmor.png", L"Cloth_Armor");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/ShieldEaring.png", L"Shield_Earings");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/PanLid.png", L"Pan_Lid");
}

void CItemData::InitializeItemInfo()
{
	// 정밀
	// 0. 발리송
	EmplaceItemInfo(0, L"Balisong", L"Precision", L"발리송", L"정밀", CreateStrDescriptionVec(0), CreateStatVec(0), ITEM_CATEGORY::PRECISION);
	// 1. 열망의 부적
	EmplaceItemInfo(1, L"Charm_Of_Aspiration", L"Precision", L"열망의 부적", L"정밀", CreateStrDescriptionVec(1), CreateStatVec(1), ITEM_CATEGORY::PRECISION);
	// 2. 뾰족한 방망이
	EmplaceItemInfo(2, L"Pointed_Club", L"Precision", L"뾰족한 방망이", L"정밀", CreateStrDescriptionVec(2), CreateStatVec(2), ITEM_CATEGORY::PRECISION);

	// 그림자
	// 3. 부서진 사파이어
	EmplaceItemInfo(3, L"Broken_Sapphire", L"Shadow", L"부서진 사파이어", L"그림자", CreateStrDescriptionVec(3), CreateStatVec(3), ITEM_CATEGORY::SHADOW);
	// 4. 환락의 망토
	EmplaceItemInfo(4, L"Cloak_Of_Verdant_Spirit", L"Shadow", L"환락의 망토", L"그림자", CreateStrDescriptionVec(4), CreateStatVec(4), ITEM_CATEGORY::SHADOW);
	// 5. 모형 부리
	EmplaceItemInfo(5, L"Model_Beak", L"Shadow", L"모형 부리", L"그림자", CreateStrDescriptionVec(5), CreateStatVec(5), ITEM_CATEGORY::SHADOW);

	// 수호
	// 6. 천 갑옷
	EmplaceItemInfo(6, L"Cloth_Armor", L"Guardian", L"천 갑옷", L"수호", CreateStrDescriptionVec(6), CreateStatVec(6), ITEM_CATEGORY::GUARDIAN);
	// 7. 방패 귀고리
	EmplaceItemInfo(7, L"Shield_Earings", L"Guardian", L"방패 귀고리", L"수호", CreateStrDescriptionVec(7), CreateStatVec(7), ITEM_CATEGORY::GUARDIAN);
	// 8. 냄비 뚜껑
	EmplaceItemInfo(8, L"Pan_Lid", L"Guardian", L"냄비 뚜껑", L"수호", CreateStrDescriptionVec(8), CreateStatVec(8), ITEM_CATEGORY::GUARDIAN);
}