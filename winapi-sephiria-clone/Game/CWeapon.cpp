#include "pch.h"
#include "CWeapon.h"

CWeapon::CWeapon()
{
	ZeroMemory(&m_tAtk, sizeof(ATK_INFO));
}

CWeapon::~CWeapon()
{
}