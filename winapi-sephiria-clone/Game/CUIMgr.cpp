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
#include "CArtifactToolTip.h"
#include "CStoneTabletToolTip.h"
#include "CShopTable.h"
#include "CMsgBox.h"
#include "CLevelUp.h"
#include "CItemSelectUI.h"
#include "CForgeUI.h"

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

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Frame.png", L"BasicInfo_Frame");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/HP_Bar_Fill.png", L"HP_Bar");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/MP_Bar_Fill.png", L"MP_Bar");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Dash_Fill.png", L"Dash_Fill");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Dash_Blank.png", L"Dash_Blank");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Exp_Bar.png", L"Exp_Bar");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Exp_Bar_Fill.png", L"Exp_Bar_Fill");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Leaf.png", L"HUD_Leaf");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/BasicInfo/Dice.png", L"HUD_Dice");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventoryBase.png", L"Inventory_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_Blank.png", L"Inventory_Slot_Blank");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_Artifact.png", L"Inventory_Slot_Artifact");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Inventory/InventorySlot_StoneTablet.png", L"Inventory_Slot_StoneTablet");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemToolTip/ArtifactToolTip/ArtifactToolTip_Base.png", L"ArtifactToolTip_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemToolTip/StoneTabletToolTip/StoneTabletToolTip_Base.png", L"StoneTabletToolTip_Base");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemToolTip/StoneTabletToolTip/StoneTabletToolTip_Slot.png", L"StoneTabletToolTip_Slot");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Shop/ShopListBase.png", L"ShopList");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Shop/ShopListSlot.png", L"ShopListSlot");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Shop/Leaf.png", L"Leaf");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Shop/Leaf_Big.png", L"Leaf_Big");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/MsgBox/MsgBoxFrame.png", L"MsgBoxFrame");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Button/Button.png", L"Button");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Button/EscapeButton.png", L"EscapeButton");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/Button/BlueButton.png", L"BlueButton");
	
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/LevelUp/LevelUp.png", L"LevelUp");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/KeyUI.png", L"KeyUI");

	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemSelect/Sephirite_Broken.png", L"Sephirite_Broken");
	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ItemSelect/Sephirite_Slot.png", L"Sephirite_Slot");


	CImgMgr::GetInstance()->InsertImg(L"../Resource/Image/UI/ForgeSlot/Forge_Slot.png", L"Forge_Slot");
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

	if (eID == UIID::INVENTORY || eID == UIID::ITEM_SELECT)
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

	if (eID == UIID::INVENTORY || eID == UIID::ITEM_SELECT)
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


	if (eID == UIID::INVENTORY || eID == UIID::ITEM_SELECT)
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
	case UIID::ARTIFACT_TOOLTIP:
		pUI = CAbstractFactory<CArtifactToolTip>::CreateUI(m_pMouse);
		break;
	case UIID::STONE_TABLET_TOOLTIP:
		pUI = CAbstractFactory<CStoneTabletToolTIp>::CreateUI(m_pMouse);
		break;
	case UIID::SHOP_TABLE:
		pUI = CAbstractFactory<CShopTable>::CreateUI(m_pMouse);
		break;
	case UIID::MSG_BOX:
		pUI = CAbstractFactory<CMsgBox>::CreateUI(m_pMouse);
		break;
	case UIID::LEVEL_UP:
		pUI = CAbstractFactory<CLevelUp>::CreateUI(m_pMouse);
		break;
	case UIID::ITEM_SELECT:
		pUI = CAbstractFactory<CItemSelectUI>::CreateUI(m_pMouse);
		break;
	case UIID::FORGE:
		pUI = CAbstractFactory<CForgeUI>::CreateUI(m_pMouse);
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

