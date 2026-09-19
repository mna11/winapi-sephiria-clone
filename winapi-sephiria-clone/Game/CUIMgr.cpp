#include "pch.h"
#include "CUIMgr.h"

#include "CObjMgr.h"
#include "CAbstractFactory.h"

#include "CUI.h"
#include "CBasicInfo.h"

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
	case UIID::BASIC_INFO:
		pUI = static_cast<CUI*>(CAbstractFactory<CBasicInfo>::CreateObj());
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

