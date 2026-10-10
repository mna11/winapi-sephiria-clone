#include "pch.h"
#include "CArtifactData.h"

#include "CImgMgr.h"

CArtifactData* CArtifactData::m_pInstance = nullptr;

CArtifactData::CArtifactData()
{
}

CArtifactData::~CArtifactData()
{
	Release();
}

void CArtifactData::Initialize()
{
	InitializeArtifactInfo();
	InitializeImg();
}

void CArtifactData::Release()
{
	m_mapArtifactInfo.clear();
}

void CArtifactData::EmplaceArtifactInfo(int iID, wstring strImg, wstring strCategoryImg, wstring strName, wstring strCategoryName, vector<wstring> vecStrDescription, vector<tagStat> vecStat, ARTIFACT_CATEGORY eCategory, int iLeaf)
{
	ARTIFACT_INFO tArtifactInfo;
	tArtifactInfo.iID = iID;
	tArtifactInfo.strImg = move(strImg);
	tArtifactInfo.strCategoryImg = move(strCategoryImg);

	tArtifactInfo.strName = move(strName);
	tArtifactInfo.strCategoryName = move(strCategoryName);

	tArtifactInfo.vecStrDescription = move(vecStrDescription);
	tArtifactInfo.vecStat = move(vecStat);

	tArtifactInfo.eCategory = eCategory;
	tArtifactInfo.iLeaf = iLeaf;

	m_mapArtifactInfo.emplace(iID, move(tArtifactInfo));
}

vector<STAT> CArtifactData::CreateStatVec(int iID)
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
		vecStat[0].fCriticalChange = 0.03f;
		vecStat[1].fCriticalChange = 0.06f;
		vecStat[2].fCriticalChange = 0.1f;
		vecStat[3].fCriticalChange = 0.14f;
		vecStat[4].fCriticalChange = 0.2f;
		break;
	case 2: // 뾰족한 방망이
		vecStat.resize(3);
		vecStat[0].fCriticalDamage = 0.2f;
		vecStat[1].fCriticalDamage = 0.4f;
		vecStat[2].fCriticalDamage = 0.6f;
		break;
	case 3: // 부서진 사파이어
		vecStat.resize(4);
		vecStat[0].iEvasion = 2;
		vecStat[0].fMoveSpeed = 0.05f;

		vecStat[1].iEvasion = 3;
		vecStat[1].fMoveSpeed = 0.07f;

		vecStat[2].iEvasion = 4;
		vecStat[2].fMoveSpeed = 0.09f;

		vecStat[3].iEvasion = 5;
		vecStat[3].fMoveSpeed = 0.12f;
		break;
	case 4: // 환락의 망토
		vecStat.resize(3);
		vecStat[0].iMaxDash = 0;
		vecStat[0].iEvasion = 4;
		vecStat[0].fCriticalChange = 0.01f;

		vecStat[1].iMaxDash = 1;
		vecStat[1].iEvasion = 6;
		vecStat[1].fCriticalChange = 0.02f;

		vecStat[2].iMaxDash = 1;
		vecStat[2].iEvasion = 8;
		vecStat[2].fCriticalChange = 0.04f;
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

vector<wstring> CArtifactData::CreateStrDescriptionVec(int iID)
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

const ARTIFACT_INFO* CArtifactData::FindArtifactInfo(int iID) const
{
	auto	iter = m_mapArtifactInfo.find(iID); 

	if (iter == m_mapArtifactInfo.end())
		return nullptr;

	return &iter->second;
}

ITEM_INFO CArtifactData::FindItemInfo(int iID)
{
	const ARTIFACT_INFO* pArtifactInfo = FindArtifactInfo(iID);

	if (nullptr == pArtifactInfo)
		return ITEM_INFO{ -1, ITEM_TYPE::END, {}, {}, - 1 };

	return ITEM_INFO{ iID, ITEM_TYPE::ARTIFACT, pArtifactInfo->strName, pArtifactInfo->strImg, pArtifactInfo->iLeaf };
}


void CArtifactData::InitializeImg()
{
	// 시너지 아이콘
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Academy.png", L"Academy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Alchemy.png", L"Alchemy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Companion.png", L"Companion");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Curse.png", L"Curse");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Ember.png", L"Ember");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/FrostRelic.png", L"FrostRelic");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Glacier.png", L"Glacier");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Guardian.png", L"Guardian");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Lake.png", L"Lake");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Magitech.png", L"Magitech");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Mystic.png", L"Mystic");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Negotiation.png", L"Negotiation");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Planet.png", L"Planet");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Precision.png", L"Precision");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Shadow.png", L"Shadow");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/SolarBlade.png", L"SolarBlade");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/StormCloud.png", L"StormCloud");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/Sturdy.png", L"Sturdy");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Category/WindSong.png", L"WindSong");

	// 아이템 아이콘
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/Balisong.png", L"Balisong");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/CharmOfAspiration.png", L"Charm_Of_Aspiration");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/PointedClub.png", L"Pointed_Club");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/BrokenSapphire.png", L"Broken_Sapphire");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/CloakOfVerdantSpirit.png", L"Cloak_Of_Verdant_Spirit");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/ModelBeak.png", L"Model_Beak");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/ClothArmor.png", L"Cloth_Armor");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/ShieldEaring.png", L"Shield_Earings");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Item/Artifact/PanLid.png", L"Pan_Lid");
}

void CArtifactData::InitializeArtifactInfo()
{
	// 정밀
	// 0. 발리송
	EmplaceArtifactInfo(0, L"Balisong", L"Precision", L"발리송", L"정밀", CreateStrDescriptionVec(0), CreateStatVec(0), ARTIFACT_CATEGORY::PRECISION, 200);
	// 1. 열망의 부적
	EmplaceArtifactInfo(1, L"Charm_Of_Aspiration", L"Precision", L"열망의 부적", L"정밀", CreateStrDescriptionVec(1), CreateStatVec(1), ARTIFACT_CATEGORY::PRECISION, 300);
	// 2. 뾰족한 방망이
	EmplaceArtifactInfo(2, L"Pointed_Club", L"Precision", L"뾰족한 방망이", L"정밀", CreateStrDescriptionVec(2), CreateStatVec(2), ARTIFACT_CATEGORY::PRECISION, 400);

	// 그림자
	// 3. 부서진 사파이어
	EmplaceArtifactInfo(3, L"Broken_Sapphire", L"Shadow", L"부서진 사파이어", L"그림자", CreateStrDescriptionVec(3), CreateStatVec(3), ARTIFACT_CATEGORY::SHADOW, 250);
	// 4. 환락의 망토
	EmplaceArtifactInfo(4, L"Cloak_Of_Verdant_Spirit", L"Shadow", L"환락의 망토", L"그림자", CreateStrDescriptionVec(4), CreateStatVec(4), ARTIFACT_CATEGORY::SHADOW, 350);
	// 5. 모형 부리
	EmplaceArtifactInfo(5, L"Model_Beak", L"Shadow", L"모형 부리", L"그림자", CreateStrDescriptionVec(5), CreateStatVec(5), ARTIFACT_CATEGORY::SHADOW, 450);

	// 수호
	// 6. 천 갑옷
	EmplaceArtifactInfo(6, L"Cloth_Armor", L"Guardian", L"천 갑옷", L"수호", CreateStrDescriptionVec(6), CreateStatVec(6), ARTIFACT_CATEGORY::GUARDIAN, 300);
	// 7. 방패 귀고리
	EmplaceArtifactInfo(7, L"Shield_Earings", L"Guardian", L"방패 귀고리", L"수호", CreateStrDescriptionVec(7), CreateStatVec(7), ARTIFACT_CATEGORY::GUARDIAN, 400);
	// 8. 냄비 뚜껑
	EmplaceArtifactInfo(8, L"Pan_Lid", L"Guardian", L"냄비 뚜껑", L"수호", CreateStrDescriptionVec(8), CreateStatVec(8), ARTIFACT_CATEGORY::GUARDIAN, 500);
}