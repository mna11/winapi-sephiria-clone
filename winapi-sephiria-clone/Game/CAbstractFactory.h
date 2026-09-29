#pragma once

#include "CObj.h"
#include "CUI.h"
#include "CWeapon.h"
#include "CWeaponController.h"
#include "CSwordAndShield.h"
#include "CSword.h"
#include "CShield.h"
#include "CItem.h"
#include "CInventory.h"

template<typename T>
class CAbstractFactory
{
public:
	static CObj* CreateObj()
	{
		CObj* pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static CObj* CreateObj(CObj* pTarget)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetTarget(pTarget);

		return pObj;
	}

	static CObj* CreateObj(float fX, float fY, float fAngle = 0.f)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(fX, fY);
		pObj->SetAngle(fAngle);

		return pObj;
	}

	static CWeapon* CreateWeapon()
	{
		return static_cast<CWeapon*>(CAbstractFactory<T>::CreateObj());
	}

	static CWeapon* CreateWeapon(CObj* pTarget)
	{
		return static_cast<CWeapon*>(CAbstractFactory<T>::CreateObj(pTarget));
	}

	static CWeaponController* CreateWeaponController(CObj* pTarget)
	{
		return static_cast<CWeaponController*>(CAbstractFactory<T>::CreateObj(pTarget));
	}

	static CObj* CreateSwordAndShield(SWORD_AND_SHIELD_STATE* pState, ATK_INFO* pAtk)
	{
		T* pSwordOrShield = static_cast<T*>(CAbstractFactory<T>::CreateObj());
		pSwordOrShield->SetState(pState);
		pSwordOrShield->SetAtkInfo(pAtk);
		return static_cast<CObj*>(pSwordOrShield);
	}

	static CObj* CreateBullet(VEC vPoint, const TCHAR* pFrameKey, CObj* pTarget)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(vPoint.fX, vPoint.fY);
		pObj->SetTarget(pTarget);
		pObj->SetFrameKey(pFrameKey);
		return pObj;
	}

	static CItem* CreateItem(int iID)
	{
		CItem* pItem = new T;
		pItem->Initialize();
		pItem->InitializeData(iID);
		return pItem;
	}

	static CInventory* CreateInventory(CPlayer* pPlayer)
	{
		CInventory* pInven = new T;
		pInven->SetOwner(pPlayer);
		pInven->Initialize();
		return pInven;
	}

	static CUI* CreateUI(CMouse* pMouse)
	{
		CUI* pUI = new T;
		pUI->Initialize();
		pUI->SetMouse(pMouse);
		return pUI;
	}

	static CUI* CreateUI(float fX, float fY, CMouse* pMouse)
	{
		CUI* pUI = new T;
		pUI->Initialize();
		pUI->SetPos(fX, fY);
		pUI->SetMouse(pMouse);
		return pUI;
	}
};

