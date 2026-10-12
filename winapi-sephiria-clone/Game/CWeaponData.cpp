#include "pch.h"
#include "CWeaponData.h"

#include "CImgMgr.h"

CWeaponData* CWeaponData::m_pInstance = nullptr;

CWeaponData::CWeaponData()
{
}

CWeaponData::~CWeaponData()
{
	Release();
}

void CWeaponData::Initialize()
{
	InitializeItemInfo();
	InitializeImg();
}

void CWeaponData::Release()
{
	m_mapWeaponInfo.clear();
}

void CWeaponData::EmplaceWeaponInfo(int iID, wstring strIconImg, wstring strName, wstring strDescription, WEAPON_TYPE eWeaponType)
{
	WEAPON_INFO tWeaponInfo;
	tWeaponInfo.iID = iID;
	tWeaponInfo.strIconImg = move(strIconImg);

	tWeaponInfo.strName = move(strName);
	tWeaponInfo.strDescription = move(strDescription);

	tWeaponInfo.eWeaponType = eWeaponType;

	m_mapWeaponInfo.emplace(iID, move(tWeaponInfo));
}

void CWeaponData::InitializeImg()
{
	// 한손검
	// 0. 일반 한손검
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/Normal/Icon.png", L"Normal_Sword_Shield_Icon");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/Normal/Sword.png", L"Normal_Sword_Shield_Sword");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/Normal/Shield.png", L"Normal_Sword_Shield_Shield");
	// 1. 매직 완드 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/MagicWand/Icon.png", L"Magic_Wand_Icon");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/MagicWand/Staff.png", L"Magic_Wand_Staff");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/MagicWand/Shield.png", L"Magic_Wand_Shield");
	// 2. 회귀하는 수호
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/ReturningGuardian/Icon.png", L"Returning_Guardian_Icon");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/ReturningGuardian/Sword.png", L"Returning_Guardian_Sword");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/SwordShield/ReturningGuardian/Shield.png", L"Returning_Guardian_Shield");
	// 낚시대
	// 0. 일반 낚시대
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/FishingRod/Normal/Icon.png", L"Fishing_Rod_Icon");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/Weapon/FishingRod/Normal/Rod.png", L"Fishing_Rod_Rod");
}

void CWeaponData::InitializeItemInfo()
{
	// 한손검
	// 0. 일반 한손검
	EmplaceWeaponInfo(0, L"Normal_Sword_Shield_Icon", L"일반 한손검", L"공격과 방어가 균형 잡힌 일반적인 한손검입니다.\n3연속 베기 / 방어 중 공격 시 돌진 회전 베기", WEAPON_TYPE::SWORD_AND_SHIELD);
	// 1. 매직 완드
	EmplaceWeaponInfo(1, L"Magic_Wand_Icon", L"매직 완드", L"화염 속성 매직 미사일을 발사합니다.\n연격마다 1 / 2 / 3발 발사 / 방어 중 공격 시 8방향 발사", WEAPON_TYPE::SWORD_AND_SHIELD);
	// 2. 회귀하는 수호
	EmplaceWeaponInfo(2, L"Returning_Guardian_Icon", L"회귀하는 수호", L"특수 공격이 방패 던지기로 변경됩니다.\n3연속 베기 / 방어 중 공격 시 방패 던지기", WEAPON_TYPE::SWORD_AND_SHIELD);
	// 낚시대
	// 0. 일반 낚시대
	EmplaceWeaponInfo(3, L"Fishing_Rod_Icon", L"일반 낚시대", L"가볍고 튼튼해 누구나 사용하기 좋은 낚시대입니다.\n입질에 맞춰 낚아채고, 줄의 장력을 조절해 물고기를 낚습니다.", WEAPON_TYPE::FISHING_ROD);
}

const WEAPON_INFO* CWeaponData::FindWeaponInfo(int iID) const
{
	auto	iter = m_mapWeaponInfo.find(iID);

	if (iter == m_mapWeaponInfo.end())
		return nullptr;

	return &iter->second;
}
