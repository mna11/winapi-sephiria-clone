#include "pch.h"
#include "CUIMgr.h"

#include "CObjMgr.h"
#include "CPlayer.h"
#include "CMouse.h"
#include "CAbstractFactory.h"
#include "CImgMgr.h"

#include "CUI.h"
#include "CBasicInfo.h"
#include "CBossHp.h"
#include "CInventoryUI.h"
#include "CItemToolTip.h"
#include "CShopTable.h"
#include "CMsgBox.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
	: m_mapUI{}, m_pMouse(nullptr)
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::Initialize()
{
	// 스프라이트 넣기 
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventoryBase.png", L"Inventory_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_Blank.png", L"Inventory_Slot_Blank");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_Item.png", L"Inventory_Slot_Item");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemToolTip/ItemToolTip_Base.png", L"ItemToolTip_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Shop/ShopListBase.png", L"ShopList");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Shop/ShopListSlot.png", L"ShopListSlot");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/MsgBox/MsgBoxFrame.png", L"MsgBoxFrame");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Button/Button.png", L"Button");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Button/EscapeButton.png", L"EscapeButton");
}

void CUIMgr::ShowUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer || nullptr == m_pMouse)
		return;

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		if (nullptr == pUI)
			return;
		pUI->Show();
	}
	else
		(*iter).second->Show();

	if (eID == UIID::INVENTORY)
	{
		pPlayer->SetPlayerBehaviorEnable(false);
		m_pMouse->RequestChange(MOUSE_STATE::UI_IDLE);
	}
}

void CUIMgr::ShowUI(UIID eID, CObj* pTarget)
{
	auto iter = m_mapUI.find(eID);

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		if (nullptr == pUI)
			return;
		pUI->Show();
		pUI->SetTarget(pTarget);
	}
	else
	{
		(*iter).second->Show();
		(*iter).second->SetTarget(pTarget);
	}

	if (eID == UIID::INVENTORY)
	{
		pPlayer->SetPlayerBehaviorEnable(false);
		m_pMouse->RequestChange(MOUSE_STATE::UI_IDLE);
	}
}

void CUIMgr::HideUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);
	if (iter == m_mapUI.end())
		return;

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	(*iter).second->Hide();

	if (eID == UIID::INVENTORY)
	{
		pPlayer->SetPlayerBehaviorEnable(true);
		m_pMouse->RequestChange(MOUSE_STATE::COMBAT);
	}
}

void CUIMgr::HideAllUI()
{
	for (int i = 0; i < toUType(UIID::END); ++i)
	{
		HideUI(static_cast<UIID>(i));
	}
}

void CUIMgr::ToggleUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		if (nullptr == pUI)
			return;
		pUI->Toggle();
	}
	else
		(*iter).second->Toggle();


	if (eID == UIID::INVENTORY)
	{
		bool bView = m_mapUI[eID]->GetView();
		pPlayer->SetPlayerBehaviorEnable(!bView);
		if (bView)
			m_pMouse->RequestChange(MOUSE_STATE::UI_IDLE);
		else 
			m_pMouse->RequestChange(MOUSE_STATE::COMBAT);
	}
}

CUI* CUIMgr::GetUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		return pUI;
	}
	else
	{
		return (*iter).second;
	}
}

void CUIMgr::SetPos(UIID eID, VEC vPoint)
{
	CUI* pUI = GetUI(eID);
	if (nullptr != pUI)
		pUI->SetPos(vPoint);
}

CUI* CUIMgr::CreateUI(UIID eID)
{
	CUI* pUI(nullptr);

	switch (eID)
	{
	case UIID::BASIC_INFO: // 플레이어 체력 마나 대시 바 (스테이지에서 왼쪽 상단에 있는거)
		pUI = CAbstractFactory<CBasicInfo>::CreateUI(m_pMouse);
		break;
	case UIID::BOSS_HP: // 에르마 체력 바
		pUI = CAbstractFactory<CBossHp>::CreateUI(m_pMouse);
		break;
	case UIID::INVENTORY:
		// 인벤토리UI를 생성할 때, 플레이어의 인벤토리를 연결해줌
		pUI = CAbstractFactory<CInventoryUI>::CreateUI(m_pMouse);
		static_cast<CInventoryUI*>(pUI)->SetInventory(CObjMgr::GetInstance()->GetPlayer()->GetInventory());
		break;
	case UIID::ITEM_TOOLTIP:
		pUI = CAbstractFactory<CItemToolTip>::CreateUI(m_pMouse);
		break;
	case UIID::SHOP_TABLE:
		pUI = CAbstractFactory<CShopTable>::CreateUI(m_pMouse);
		break;
	case UIID::MSG_BOX:
		pUI = CAbstractFactory<CMsgBox>::CreateUI(m_pMouse);
		break;
	default:
		break;
	}

	if (nullptr != pUI)
	{
		m_mapUI.emplace(eID, pUI);
		CObjMgr::GetInstance()->AddObject(OBJID::UI, pUI);
	}

	return pUI;
}

