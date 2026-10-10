#include "pch.h"
#include "CWeapon.h"

CWeapon::CWeapon()
	: m_eWeaponType(WEAPON_TYPE::END), m_iID(-1), m_iMpCost(0)
{
	ZeroMemory(&m_tAtk, sizeof(ATK_INFO));
}

CWeapon::~CWeapon()
{
}