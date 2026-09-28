#include "pch.h"
#include "CUIMgr.h"

#include "CObjMgr.h"
#include "CAbstractFactory.h"

#include "CUI.h"
#include "CBasicInfo.h"
#include "CBossHp.h"

CUIMgr* CUIMgr::m_pInstance = nullptr;

CUIMgr::CUIMgr()
	: m_mapUI{}
{
}

CUIMgr::~CUIMgr()
{
}

void CUIMgr::ShowUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		pUI->Show();
	}
	else
		(*iter).second->Show();
}

void CUIMgr::ShowUI(UIID eID, CObj* pTarget)
{
	auto iter = m_mapUI.find(eID);

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
}

void CUIMgr::HideUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		pUI->Hide();
	}
	else
		(*iter).second->Hide();
}

void CUIMgr::ToggleUI(UIID eID)
{
	auto iter = m_mapUI.find(eID);

	if (iter == m_mapUI.end())
	{
		CUI* pUI = CreateUI(eID);
		pUI->Toggle();
	}
	else
		(*iter).second->Toggle();
}

CUI* CUIMgr::CreateUI(UIID eID)
{
	CUI* pUI(nullptr);

	switch (eID)
	{
	case UIID::BASIC_INFO: // 플레이어 체력 마나 대시 바 (스테이지에서 왼쪽 상단에 있는거)
		pUI = static_cast<CUI*>(CAbstractFactory<CBasicInfo>::CreateObj());
		break;
	case UIID::BOSS_HP: // 에르마 체력 바
		pUI = static_cast<CUI*>(CAbstractFactory<CBossHp>::CreateObj());
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

