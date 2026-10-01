#pragma once

#include "CUI.h"

class CMouse;

class CUIMgr
{
private:
	CUIMgr();
	~CUIMgr();
	CUIMgr(const CUIMgr& rhs) = delete;
	CUIMgr& operator=(CUIMgr& rUIMgr) = delete;

public:
	static CUIMgr* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CUIMgr;
			m_pInstance->Initialize();
		}

		return m_pInstance;
	}

	static void	DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}
public:
	void Initialize();

public:
	void ShowUI(UIID eID);
	void ShowUI(UIID eID, CObj* pTarget);
	void HideUI(UIID eID);
	void HideAllUI();
	void ToggleUI(UIID eID);

public:
	CUI* GetUI(UIID eID);

public:
	void SetMouse(CMouse* pMouse) { m_pMouse = pMouse; }
	void SetPos(UIID eID, VEC vPoint);

private:
	CUI* CreateUI(UIID eID);

private:
	static CUIMgr* m_pInstance;
	map<UIID, CUI*> m_mapUI;

	CMouse* m_pMouse;
};

