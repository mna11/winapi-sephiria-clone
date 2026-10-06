#pragma once

#include "CObj.h"
#include "CBullet.h"
#include "CDrop.h"
#include "CUI.h"
#include "CWeapon.h"
#include "CWeaponController.h"
#include "CSwordAndShield.h"
#include "CSword.h"
#include "CShield.h"
#include "CItem.h"
#include "CScene.h"
#include "CInventory.h"
#include "CButton.h"
#include "CMsgBox.h"

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

	static CObj* CreateBullet(VEC vPoint, const TCHAR* pFrameKey, float fAngle, float fSpeed, float fDamage, CObj* pOwner)
	{
		CBullet* pObj = new T;
		pObj->Initialize();
		pObj->SetPos(vPoint.fX, vPoint.fY);
		pObj->SetFrameKey(pFrameKey);
		pObj->UpdateInfo();
		pObj->SetAngle(fAngle);
		pObj->SetSpeed(fSpeed);
		pObj->SetOwner(pOwner);
		pObj->SetDamage(fDamage);
		return pObj;
	}

	static CItem* CreateItem(int iID, CPlayer* pPlayer)
	{
		CItem* pItem = new T;
		pItem->Initialize();
		pItem->InitializeData(iID);
		pItem->SetTarget(pPlayer);
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

	static CScene* CreateScene()
	{
		CScene* pScene = new T;
		pScene->Initialize();
		return pScene;
	}

	static CButton* CreateButton(CMouse* pMouse, RectF rcDest, wstring wstr,
		 const TCHAR* pFrameKey = L"Button", VEC vCellSize = { 61.f, 24.f })
	{
		CButton* pButton = new T;
		pButton->Initialize();
		pButton->SetMouse(pMouse);
		pButton->SetPrintRect(rcDest);
		pButton->SetString(wstr);
		pButton->SetFrameKey(pFrameKey);
		pButton->SetCellSize(vCellSize);
		pButton->Show();
		return pButton;
	}

	static CMsgBox* CreateMsgBox(RectF rcDest, int iBtnNum, MSG_BOX_LAYOUT eLayout, wstring wstr)
	{
		CMsgBox* pMsgBox = new T;
		pMsgBox->Initialize();
		pMsgBox->SetPrintRect(rcDest);
		pMsgBox->SetLayout(eLayout);
		pMsgBox->SetString(move(wstr));
		pMsgBox->SetButtonNumber(iBtnNum);
		pMsgBox->UpdateLayout();
		pMsgBox->Show();

		return pMsgBox;
	}

	static CObj* CreateDrop(float fX, float fY, CObj* pTarget, DROP_TYPE eDropType, int iAmount)
	{
		CDrop* pDrop = new T;
		pDrop->Initialize();
		pDrop->SetPos(fX, fY);
		pDrop->SetTarget(pTarget);
		pDrop->SetDropType(eDropType);
		pDrop->SetAmount(iAmount);
		return pDrop;
	}
};

