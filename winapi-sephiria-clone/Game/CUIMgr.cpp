#include "pch.h"
#include "CUIMgr.h"

#include "CObjMgr.h"
#include "CPlayer.h"
#include "CMouse.h"
#include "CAbstractFactory.h"

#include "CUI.h"
#include "CBasicInfo.h"
#include "CBossHp.h"
#include "CInventoryUI.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
	: m_mapUI{}, m_pMouse(nullptr)
{
}

CUIMgr::~CUIMgr()
{
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
		pUI->Show();
	}
	else
		(*iter).second->Show();

	if (eID == UIID::INVENTORY)
	{
		pPlayer->SetPlayerBehaviorEnable(true);
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
		pPlayer->SetPlayerBehaviorEnable(true);
		m_pMouse->RequestChange(MOUSE_STATE::UI_IDLE);
	}
}

void CUIMgr::HideUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	CPlayer* pPlayer = CObjMgr::GetInstance()->GetPlayer();
	if (nullptr == pPlayer)
		return;

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		pUI->Hide();
	}
	else
		(*iter).second->Hide();

	if (eID == UIID::INVENTORY)
	{
		pPlayer->SetPlayerBehaviorEnable(true);
		m_pMouse->RequestChange(MOUSE_STATE::COMBAT);
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
		(*iter).second->Toggle();
		return (*iter).second;
	}
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

