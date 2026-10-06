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

void CWeaponData::EmplaceWeaponInfo(int iID, wstring strIconImg, wstring strImg1, wstring strImg2, wstring strName, wstring strDescription, WEAPON_TYPE eWeaponType)
{
	WEAPON_INFO tWeaponInfo;
	tWeaponInfo.iID = iID;
	tWeaponInfo.strIconImg = move(strIconImg);
	tWeaponInfo.strImg1 = move(strImg1);
	tWeaponInfo.strImg2 = move(strImg2);

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
}

void CWeaponData::InitializeItemInfo()
{
	// 한손검
	// 0. 일반 한손검
	EmplaceWeaponInfo(0, L"Normal_Sword_Shield_Icon", L"Normal_Sword_Shield_Sword", L"Normal_Sword_Shield_Shield", L"일반 한손검", L"일반적인 한손검입니다", WEAPON_TYPE::SWORD_AND_SHIELD);
	// 1. 매직 완드
	EmplaceWeaponInfo(1, L"Magic_Wand_Icon", L"Magic_Wand_Staff", L"Magic_Wand_Shield", L"매직 완드", L"무기 공격이 매직 미사일을 사용하는 형태로 변경됩니다.", WEAPON_TYPE::SWORD_AND_SHIELD);
}

const WEAPON_INFO* CWeaponData::FindWeaponInfo(int iID) const
{
	auto	iter = m_mapWeaponInfo.find(iID);

	if (iter == m_mapWeaponInfo.end())
		return nullptr;

	return &iter->second;
}
